/*
 * alert tcp $EXTERNAL_NET $HTTP_PORTS -> $HOME_NET any (msg:"WEB-CLIENT RealNetworks RealPlayer wav chunk string overflow attempt"; flow:to_client,established; flowbits:isset,file.wav; content:"LIST"; content:"INFO"; distance:4; within:4; metadata:policy balanced-ips drop, policy security-ips drop, service http; classtype:attempted-user; reference:cve,2005-0611; reference:bugtraq,12697; sid:!;)
 *
 */
 /*
 * This code was automatically generated by the Sourcefire VRT Rules2C web generator
 *
 * We make no claims to the correctness or fitness of the code generated or of modifications
 * to the generated code.
 *
 * Use at your own risk.
 *
 * Please report any problems with this tool to research@sourcefire.com
 */

#include "sf_snort_plugin_api.h"
#include "sf_snort_packet.h"


/* declare detection functions */
int rule17700eval(void *p);

/* declare rule data structures */
/* flow:established, to_client; */
static FlowFlags rule17700flow0 = 
{
    FLOW_ESTABLISHED|FLOW_TO_CLIENT
};

static RuleOption rule17700option0 =
{
    OPTION_TYPE_FLOWFLAGS,
    {
        &rule17700flow0
    }
};
/* flowbits:isset "file.wav"; */
static FlowBitsInfo rule17700flowbits1 =
{
    "file.wav",
    FLOWBIT_ISSET,
    0,
};

static RuleOption rule17700option1 =
{
    OPTION_TYPE_FLOWBIT,
    {
        &rule17700flowbits1
    }
};
// content:"LIST", payload raw, depth 0, fast_pattern; 
static ContentInfo rule17700content2 = 
{
    (u_int8_t *) "LIST", /* pattern (now in snort content format) */
    0, /* depth */
    0, /* offset */
    CONTENT_FAST_PATTERN|CONTENT_BUF_NORMALIZED, /* flags */
    NULL, /* holder for boyer/moore PTR */
    NULL, /* more holder info - byteform */
    0, /* byteform length */
    0 /* increment length*/
};

static RuleOption rule17700option2 = 
{
    OPTION_TYPE_CONTENT,
    {
        &rule17700content2
    }
};
// content:"INFO", payload raw, offset 4, depth 4, relative; 
static ContentInfo rule17700content3 = 
{
    (u_int8_t *) "INFO", /* pattern (now in snort content format) */
    4, /* depth */
    4, /* offset */
    CONTENT_RELATIVE|CONTENT_BUF_NORMALIZED, /* flags */
    NULL, /* holder for boyer/moore PTR */
    NULL, /* more holder info - byteform */
    0, /* byteform length */
    0 /* increment length*/
};

static RuleOption rule17700option3 = 
{
    OPTION_TYPE_CONTENT,
    {
        &rule17700content3
    }
};

/* references for sid 17700 */
/* reference: bugtraq "12697"; */
static RuleReference rule17700ref1 = 
{
    "bugtraq", /* type */
    "12697" /* value */
};

/* reference: cve "2005-0611"; */
static RuleReference rule17700ref2 = 
{
    "cve", /* type */
    "2005-0611" /* value */
};

static RuleReference *rule17700refs[] =
{
    &rule17700ref1,
    &rule17700ref2,
    NULL
};
/* metadata for sid 17700 */
/* metadata:service http, policy balanced-ips drop, policy security-ips drop; */
static RuleMetaData rule17700service1 = 
{
    "service http"
};


static RuleMetaData rule17700policy1 = 
{
    "policy balanced-ips drop"
};

static RuleMetaData rule17700policy2 = 
{
    "policy security-ips drop"
};


static RuleMetaData *rule17700metadata[] =
{
    &rule17700service1,
    &rule17700policy1,
    &rule17700policy2,
    NULL
};

RuleOption *rule17700options[] =
{
    &rule17700option0,
    &rule17700option1,
    &rule17700option2,
    &rule17700option3,
    NULL
};

Rule rule17700 = {
   /* rule header, akin to => tcp any any -> any any */
   {
       IPPROTO_TCP, /* proto */
       "$EXTERNAL_NET", /* SRCIP     */
       "$HTTP_PORTS", /* SRCPORT   */
   
       0, /* DIRECTION */
       "$HOME_NET", /* DSTIP     */
   
       "any", /* DSTPORT   */
   },
   /* metadata */
   { 
       3,  /* genid */
       17700, /* sigid */
       2, /* revision */
       "attempted-user", /* classification */
       0,  /* hardcoded priority XXX NOT PROVIDED BY GRAMMAR YET! */
       "WEB-CLIENT RealNetworks RealPlayer wav chunk string overflow attempt",     /* message */
       rule17700refs /* ptr to references */
       ,rule17700metadata
   },
   rule17700options, /* ptr to rule options */
   &rule17700eval, /* replace with NULL to use the built in detection function */
   0 /* am I initialized yet? */
};


/* detection functions */
int rule17700eval(void *p) {
    const u_int8_t *cursor_normal = 0, *beg_of_payload, *end_of_payload;
    SFSnortPacket *sp = (SFSnortPacket *) p;

    const u_int8_t *cursor_detect;

    u_int32_t fourCC;
    u_int32_t listsize;
    u_int32_t chunksize;
    u_int32_t stringsize;

    if(sp == NULL)
        return RULE_NOMATCH;

    if(sp->payload == NULL)
        return RULE_NOMATCH;

    // flow:established, to_client;
    if (checkFlow(p, rule17700options[0]->option_u.flowFlags) > 0 ) {
        // flowbits:isset "file.wav";
        if (processFlowbits(p, rule17700options[1]->option_u.flowBit) > 0) {
            // content:"LIST", payload raw, depth 0, fast_pattern;
            if (contentMatch(p, rule17700options[2]->option_u.content, &cursor_normal) > 0) {
                // content:"INFO", payload raw, offset 4, depth 4, relative;
                if (contentMatch(p, rule17700options[3]->option_u.content, &cursor_normal) > 0) {
                    
                    if (getBuffer(sp, CONTENT_BUF_NORMALIZED, &beg_of_payload, &end_of_payload) <= 0)
                        return RULE_NOMATCH;

                    //Extract size of list chunk size
                    listsize = *(cursor_normal-8);
                    listsize |= *(cursor_normal-7) << 8;
                    listsize |= *(cursor_normal-6) << 16;
                    listsize |= *(cursor_normal-5) << 24;

                    //Make sure we have 8 bytes to extact 4CC and chunk size
                    while(cursor_normal + 8 < end_of_payload) {
                        //Extract chunk size
                        chunksize = *(cursor_normal+4);
                        chunksize |= *(cursor_normal+5) << 8;
                        chunksize |= *(cursor_normal+6) << 16;
                        chunksize |= *(cursor_normal+7) << 24;

                        //In a loop, check the 4CC tag to see if it's any of the vulnerable tags
                        fourCC = *cursor_normal << 24;
                        fourCC |= *(cursor_normal + 1) << 16;
                        fourCC |= *(cursor_normal + 2) << 8;
                        fourCC |= *(cursor_normal + 3);

                        if((fourCC == 0x49415254) || // IART
                           (fourCC == 0x494e414d) || // INAM
                           (fourCC == 0x49434f50)) { // ICOP

                            //If it is one of the vulnerable tags, jump to the subchunk data
                            //and treat it as a string. Count the actual size of the string
                            //in another loop.
                            stringsize = 0;
    
                            cursor_detect = cursor_normal + 8;
    
                            //Match if stringsize > either chunksize or listsize
                            //otherwise keep counting characters or break if we
                            //encounter a null character or end_of_payload
                            while(cursor_detect < end_of_payload) {
                               if(!*cursor_detect++)
                                  break;  // stop when we find a null
                            }

                            stringsize = cursor_detect - cursor_normal - 8;

                            //printf("stringsize=%d, chunksize=%d, listsize=%d\n");
                            if (stringsize > chunksize || stringsize > listsize)
                                return RULE_MATCH;
                        }
                        
                        //We need to do a check before skipping to the next chunk to make sure that
                        //addition won't cause an integer overflow on cursor_normal
                        if (cursor_normal + chunksize + 8 <= cursor_normal) // <= to also avoid infinite loops
                            return RULE_NOMATCH;

                        // Jump to the start of the next chunk
                        cursor_normal += chunksize + 8; // fourCC, 4 bytes size, chunk size
                    } 
                }
            }
        }
    }
    return RULE_NOMATCH;
}

/*
Rule *rules[] = {
    &rule17700,
    NULL
};
*/
