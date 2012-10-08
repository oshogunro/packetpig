/* resticted */
/*
 * Use at your own risk.
 *
 * Copyright (C) 2005-2008 Sourcefire, Inc.
 * 
 * This file is autogenerated via rules2c, by Brian Caswell <bmc@sourcefire.com>
 */


#include "sf_snort_plugin_api.h"
#include "sf_snort_packet.h"


/* declare detection functions */
int rule16180eval(void *p);

/* declare rule data structures */
/* precompile the stuff that needs pre-compiled */
/* flow:established, to_client; */
static FlowFlags rule16180flow0 = 
{
    FLOW_ESTABLISHED|FLOW_TO_CLIENT
};

static RuleOption rule16180option0 =
{
    OPTION_TYPE_FLOWFLAGS,
    {
        &rule16180flow0
    }
};
// content:"|16 03|", depth 0; 
static ContentInfo rule16180content1 = 
{
    (u_int8_t *) "|16 03|", /* pattern (now in snort content format) */
    0, /* depth */
    0, /* offset */
    CONTENT_BUF_NORMALIZED, /* flags */
    NULL, /* holder for boyer/moore PTR */
    NULL, /* more holder info - byteform */
    0, /* byteform length */
    0 /* increment length*/
};

static RuleOption rule16180option1 = 
{
    OPTION_TYPE_CONTENT,
    {
        &rule16180content1
    }
};
#ifndef CONTENT_FAST_PATTERN
#define CONTENT_FAST_PATTERN 0
#endif
// content:"U|04 03|", depth 0, relative, fast_pattern; 
static ContentInfo rule16180content2 = 
{
    (u_int8_t *) "U|04 03|", /* pattern (now in snort content format) */
    0, /* depth */
    0, /* offset */
    CONTENT_RELATIVE|CONTENT_FAST_PATTERN|CONTENT_BUF_NORMALIZED, /* flags */
    NULL, /* holder for boyer/moore PTR */
    NULL, /* more holder info - byteform */
    0, /* byteform length */
    0 /* increment length*/
};

static RuleOption rule16180option2 = 
{
    OPTION_TYPE_CONTENT,
    {
        &rule16180content2
    }
};

/* references for sid 16180 */
/* reference: cve "2009-2510"; */
static RuleReference rule16180ref1 = 
{
    "cve", /* type */
    "2009-2510" /* value */
};

/* reference: url "technet.microsoft.com/en-us/security/bulletin/MS09-056"; */
static RuleReference rule16180ref2 = 
{
    "url", /* type */
    "technet.microsoft.com/en-us/security/bulletin/MS09-056" /* value */
};

static RuleReference *rule16180refs[] =
{
    &rule16180ref1,
    &rule16180ref2,
    NULL
};
/* metadata for sid 16180 */
/* metadata:service ssl, policy security-ips drop; */
static RuleMetaData rule16180service1 = 
{
    "service ssl"
};


static RuleMetaData rule16180policy1 = 
{
    "policy security-ips drop"
};


static RuleMetaData *rule16180metadata[] =
{
    &rule16180service1,
    &rule16180policy1,
    NULL
};

RuleOption *rule16180options[] =
{
    &rule16180option0,
    &rule16180option1,
    &rule16180option2,
    NULL
};

Rule rule16180 = {
   
   /* rule header, akin to => tcp any any -> any any               */{
       IPPROTO_TCP, /* proto */
       "$EXTERNAL_NET", /* SRCIP     */
       "443", /* SRCPORT   */
   
       0, /* DIRECTION */
       "$HOME_NET", /* DSTIP     */
   
       "any", /* DSTPORT   */
   },
   /* metadata */
   { 
       3,  /* genid */
       16180, /* sigid */
       5, /* revision */
   
       "misc-attack", /* classification */
       0,  /* hardcoded priority XXX NOT PROVIDED BY GRAMMAR YET! */
       "WEB-CLIENT Windows CryptoAPI common name spoofing attempt",     /* message */
       rule16180refs /* ptr to references */
       ,rule16180metadata
   },
   rule16180options, /* ptr to rule options */
   &rule16180eval, /* use the built in detection function */
   0 /* am I initialized yet? */
};


/* detection functions */
int rule16180eval(void *p) {
    const u_int8_t *cursor_normal = 0;
    SFSnortPacket *sp = (SFSnortPacket *) p;
    const u_int8_t *beg_of_payload, *end_of_payload;
    const u_int8_t *end_of_CommonName;
    const u_int8_t *cursor_detect;
    u_int8_t unicode = 0;

    if(sp == NULL)
        return RULE_NOMATCH;

    if(sp->payload == NULL)
        return RULE_NOMATCH;
    
    // flow:established, to_client;
    if (checkFlow(p, rule16180options[0]->option_u.flowFlags) > 0 ) {

        // content:"|16 03|", depth 0;
        if (contentMatch(p, rule16180options[1]->option_u.content, &cursor_normal) > 0) {

            if(getBuffer(sp, CONTENT_BUF_NORMALIZED, &beg_of_payload, &end_of_payload) <= 0)
                  return RULE_NOMATCH;

            // content:"U|04 03|", depth 0, relative, fast_pattern;
            while(contentMatch(p, rule16180options[2]->option_u.content, &cursor_normal) > 0) {
                cursor_detect = cursor_normal;

                // Skip single byte type value, we're not checking it
                // If the type value is 0x1e, it's unicode
                if(*cursor_detect++ == 0x1e)
                   unicode = 1;

                if(cursor_detect + 1 > end_of_payload)
                   return RULE_NOMATCH;

                // Extract one-byte length value and use it to set end_of_CommonName
                end_of_CommonName = cursor_detect + *cursor_detect + 1;

                // Skip length value
                cursor_detect++;

                // Ensure we don't go past the end of the payload
                if (end_of_payload < end_of_CommonName) {
                    end_of_CommonName = end_of_payload;
                }

                // Zip through the field, stop at end or NULL
                // For speed, we're doing two separate loops for unicode or regular
                if(!unicode) {
                   while ((cursor_detect < end_of_CommonName) && *cursor_detect++) {
                      // empty loop
                   }
                } else {
                   // Unicode, so both have to be null to avoid false positives
                   // I organized the code a little funny to make the while loop easier
                   cursor_detect += 2;

                   while (cursor_detect < end_of_CommonName) {
                      // We check the second character first, because that's the one that
                      // is more likely to be non-null.
                      if(*(cursor_detect - 1) || *(cursor_detect - 2)) {
                         cursor_detect += 2; 
                      } else {
                         break;
                      }
                   }
                }

                // If we stop before the end, there was a NULL so alert
                if (cursor_detect < end_of_CommonName) {
                    return RULE_MATCH;
                }
            }
        }
    }
    return RULE_NOMATCH;
}

/*
Rule *rules[] = {
    &rule16180,
    NULL
};
*/
