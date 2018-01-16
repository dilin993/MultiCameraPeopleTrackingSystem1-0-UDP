#include "core.h"

static const int defaultNMixtures = 5;
static const int defaultHistory = 200;
static const data_t defaultBackgroundRatio = 0.7;
static const data_t defaultVarThreshold = 2.5*2.5;
static const data_t defaultNoiseSigma = 30*0.5;
static const data_t defaultInitialWeight = 0.05;
const data_t w0 = defaultInitialWeight;
const data_t sk0 = w0/(defaultNoiseSigma*2);
const data_t var0 = defaultNoiseSigma*defaultNoiseSigma*4;
const data_t minVar = defaultNoiseSigma*defaultNoiseSigma;


void process(uint8_t frame_in[IMG_SIZE/PARTS],
             uint8_t frame_out[IMG_SIZE/PARTS],
			 data_t bgmodel[4*BGM_SIZE/PARTS],
			 const data_t learningRate)
{
    int x, y, k, k1, rows = HEIGHT/PARTS, cols = WIDTH;
    data_t alpha = learningRate, T = defaultBackgroundRatio, vT = defaultVarThreshold;
    data_t* mptr = bgmodel;



    for( y = 0; y < rows; y++ )
    {
        const uint8_t * src = frame_in+y*cols;
        uint8_t * dst = frame_out+y*cols;

        if( alpha > 0 )
        {
            for( x = 0; x < cols; x++, mptr += K*4)
            {
            	data_t wsum = 0;
            	data_t pix = src[x];
                int kHit = -1, kForeground = -1;

                for( k = 0; k < K; k++ )
                {
                	data_t w = mptr[k*4+1];//weight;
                    wsum += w;
                    if( w < FLT_EPSILON )
                        break;
                    data_t mu = mptr[k*4+2];//.mean;
                    data_t var = mptr[k*4+3];//.var;
                    data_t diff = pix - mu;
                    data_t d2 = diff*diff;
                    if( d2 < vT*var )
                    {
                        wsum -= w;
                        data_t dw = alpha*(1.f - w);
                        mptr[k*4+1] = w + dw;
                        mptr[k*4+2] = mu + alpha*diff;
                        var = std::max(var + alpha*(d2 - var), minVar);
                        mptr[k*4+3] = var;
                        mptr[k*4] = w/std::sqrt(var);

                        for( k1 = k-1; k1 >= 0; k1-- )
                        {
                            if( mptr[k1*4] >= mptr[k1*4+1] )
                                break;
                            std::swap( mptr[k1*4], mptr[k1*4+1] );
                        }

                        kHit = k1+1;
                        break;
                    }
                }

                if( kHit < 0 ) // no appropriate gaussian mixture found at all, remove the weakest mixture and create a new one
                {
                    kHit = k = std::min(k, (K-1));
                    wsum += w0 - mptr[k*4+1];//.weight;
                    mptr[k*4+1] = w0;
                    mptr[k*4+2] = pix;
                    mptr[k*4+3] = var0;
                    mptr[k*4] = sk0;
                }
                else
                {
                	for( ; k < K; k++ )
                	{
                		wsum += mptr[k*4+1];//.weight;
                	}
                }


                data_t wscale = 1.f/wsum;
                wsum = 0;
                for( k = 0; k < K; k++ )
                {
                    wsum += mptr[k*4+1] *= wscale;
                    mptr[k*4] *= wscale;
                    if( wsum > T && kForeground < 0 )
                        kForeground = k+1;
                }

                dst[x] = (uint8_t)(-(kHit >= kForeground));
            }
        }
        else
        {
            for( x = 0; x < cols; x++, mptr += K*4 )
            {
            	data_t pix = src[x];
                int kHit = -1, kForeground = -1;

                for( k = 0; k < K; k++ )
                {
                    if( mptr[k*4+1] < FLT_EPSILON )
                        break;
                    data_t mu = mptr[k*4+2];
                    data_t var = mptr[k*4+3];
                    data_t diff = pix - mu;
                    data_t d2 = diff*diff;
                    if( d2 < vT*var )
                    {
                        kHit = k;
                        break;
                    }
                }

                if( kHit >= 0 )
                {
                	data_t wsum = 0;
                    for( k = 0; k < K; k++ )
                    {
                        wsum += mptr[k*4+1];
                        if( wsum > T )
                        {
                            kForeground = k+1;
                            break;
                        }
                    }
                }

                dst[x] = (uint8_t)(kHit < 0 || kHit >= kForeground ? 255 : 0);
            }
        }
    }
}

void bgsub(uint8_t frame_in[IMG_SIZE],
           uint8_t frame_out[IMG_SIZE],
		   bool init,
		   float bgmodel[4*BGM_SIZE])
{
	data_t part_bgmodel[4*BGM_SIZE/PARTS];
    uint8_t part_frame_in[IMG_SIZE/PARTS];
    uint8_t part_frame_out[IMG_SIZE/PARTS];
    data_t learningRate;

    data_t part2_bgmodel[4*BGM_SIZE/PARTS];
    uint8_t part2_frame_in[IMG_SIZE/PARTS];
    uint8_t part2_frame_out[IMG_SIZE/PARTS];
    data_t learningRate2;

    data_t part3_bgmodel[4*BGM_SIZE/PARTS];
    uint8_t part3_frame_in[IMG_SIZE/PARTS];
    uint8_t part3_frame_out[IMG_SIZE/PARTS];
    data_t learningRate3;

    data_t part4_bgmodel[4*BGM_SIZE/PARTS];
    uint8_t part4_frame_in[IMG_SIZE/PARTS];
    uint8_t part4_frame_out[IMG_SIZE/PARTS];
    data_t learningRate4;

    if( init )
    {

//        MixData tmp;
//        tmp.mean = 0;
//        tmp.var = 0;
//        tmp.sortKey = 0;
//        tmp.weight = 0;
        for(int p=0;p<BGM_SIZE*4;p++)
            bgmodel[p] = 0;
//    	memset(bgmodel,0,sizeof(float)*BGM_SIZE*4);

        learningRate = 1;
        learningRate2 = 1;
        learningRate3 = 1;
        learningRate4 = 1;
    }
    else
    {
    	learningRate = 0;
    	learningRate2 = 0;
    	learningRate3 = 0;
    	learningRate4 = 0;
    }

    for(int part=0;part<PARTS;part+=4)
    {
    	read_mem:{
        memcpy(part_frame_in,&frame_in[(IMG_SIZE/PARTS)*part],sizeof(uint8_t)*(IMG_SIZE/PARTS));
        memcpy(part_bgmodel,&bgmodel[(4*BGM_SIZE/PARTS)*part],4*sizeof(data_t)*(BGM_SIZE/PARTS));
        memcpy(part2_frame_in,&frame_in[(IMG_SIZE/PARTS)*(part+1)],sizeof(uint8_t)*(IMG_SIZE/PARTS));
        memcpy(part2_bgmodel,&bgmodel[(4*BGM_SIZE/PARTS)*(part+1)],4*sizeof(data_t)*(BGM_SIZE/PARTS));
        memcpy(part3_frame_in,&frame_in[(IMG_SIZE/PARTS)*(part+2)],sizeof(uint8_t)*(IMG_SIZE/PARTS));
        memcpy(part3_bgmodel,&bgmodel[(4*BGM_SIZE/PARTS)*(part+2)],4*sizeof(data_t)*(BGM_SIZE/PARTS));
        memcpy(part4_frame_in,&frame_in[(IMG_SIZE/PARTS)*(part+3)],sizeof(uint8_t)*(IMG_SIZE/PARTS));
        memcpy(part4_bgmodel,&bgmodel[(4*BGM_SIZE/PARTS)*(part+3)],4*sizeof(data_t)*(BGM_SIZE/PARTS));
    	}

    	processing:{
        process(part_frame_in,part_frame_out, part_bgmodel,learningRate);
        process(part2_frame_in,part2_frame_out, part2_bgmodel,learningRate2);
        process(part3_frame_in,part3_frame_out, part3_bgmodel,learningRate3);
        process(part4_frame_in,part4_frame_out, part4_bgmodel,learningRate4);
    	}


    	write_mem:{
        memcpy(&bgmodel[(4*BGM_SIZE/PARTS)*part],part_bgmodel,4*sizeof(data_t)*(BGM_SIZE/PARTS));
        memcpy(&frame_out[(IMG_SIZE/PARTS)*part],part_frame_out,sizeof(uint8_t)*(IMG_SIZE/PARTS));
        memcpy(&bgmodel[(4*BGM_SIZE/PARTS)*(part+1)],part2_bgmodel,4*sizeof(data_t)*(BGM_SIZE/PARTS));
        memcpy(&frame_out[(IMG_SIZE/PARTS)*(part+1)],part2_frame_out,sizeof(uint8_t)*(IMG_SIZE/PARTS));
        memcpy(&bgmodel[(4*BGM_SIZE/PARTS)*(part+2)],part3_bgmodel,4*sizeof(data_t)*(BGM_SIZE/PARTS));
        memcpy(&frame_out[(IMG_SIZE/PARTS)*(part+2)],part3_frame_out,sizeof(uint8_t)*(IMG_SIZE/PARTS));
        memcpy(&bgmodel[(4*BGM_SIZE/PARTS)*(part+3)],part4_bgmodel,4*sizeof(data_t)*(BGM_SIZE/PARTS));
        memcpy(&frame_out[(IMG_SIZE/PARTS)*(part+3)],part4_frame_out,sizeof(uint8_t)*(IMG_SIZE/PARTS));
    	}



    }


}
