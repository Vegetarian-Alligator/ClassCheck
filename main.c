#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>

char *readbuffer;
char *combinebuffer;
///File Format Guide

///Internal Issues:

//Global variables are reqcuired because userp does not behave as expected
//Some Memory is duplicated for no fucntional reason
//The Twilio accound is hardcoded.
//The class is hardcoded!

//I Have calculus/circuits homeowork.  I will make this program more robust when there is an effecutal reason to do so.

///End internal issues

//Class CRN number
//Inform me if seats are great than....

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    static totalsize;
    //printf("------------------------\n\nFunctionCalled\n\n------------------------\n");
    if (readbuffer == NULL)
    {
        //printf("what the hell");
        readbuffer=malloc(sizeof(char) * size*nmemb);
        totalsize=size*nmemb;
        strcpy(readbuffer,contents);
        return size * nmemb;
    }
    //printf("nowwhat the hell");

    combinebuffer=readbuffer;
    totalsize+=size*nmemb;
    readbuffer=malloc(sizeof(char)*(totalsize+size*nmemb));
    strcpy(readbuffer,combinebuffer);
    strcat(readbuffer,contents);
    free(combinebuffer); //Have to store a chunk twice - kind of disgusting.
    return size * nmemb;
}

int main()
{
    readbuffer=NULL;
    CURL *curl;
    CURLcode res;
    curl=curl_easy_init();
    char *random;
    if (curl)
    {
        curl_easy_setopt(curl,CURLOPT_URL,"https://selfservice.stlcc.edu:9199/SLCC/bwckschd.p_disp_detail_sched?term_in=201510&crn_in=11171");
        //curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, random);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    //readbuffer=malloc(sizeof(char)*100);
    //strcpy(readbuffer,"This is a mere test.");
    //printf("\n\n\n------------------\nBeggining Print\n--------------\n-----\n\n\n");
    //printf("%s",readbuffer);
    //printf("\n\n\n------------------\nEnding Print\n--------------\n-----\n\n\n");

    ///Parsing Guidlines
    ///First, find: <th class="ddlabel" scope="row"><span class="fieldlabeltext">Seats</span></th>
    ///Order of seats is thusly: Capacity Actual Remaining

    char *scan;
    scan = readbuffer;
    scan = strstr(readbuffer,">Seats<");
    if (scan==NULL){
    printf("Site read error");
    exit(-1);
    }
    char *start=scan;

    printf("\n\n\n------------------\nBeggining Print\n--------------\n-----\n\n\n");
  //  printf("%s",readbuffer);
    printf("\n\n\n------------------\nEnding Print\n--------------\n-----\n\n\n");

    char numbers[3][4];

    int i;
    int t;
    for (i=0;i<4;i++) for (t=0;t<5;t++) numbers[i][t]='\0';
    for (i=0;i<3;i++) {
        scan = strstr(scan,"CLASS");
        scan = strstr(scan,">");
        start=scan;
        scan=strstr(scan,"<");
        memcpy(numbers[i],start+1,sizeof(char)*abs(start-scan)-1);
        //printf(":: %s ::\n",numbers[i]);
    }

    printf("\n\nNumber of Open seats: %s\n\n",numbers[2]);
    int num = atoi(numbers[2]);
    if (num > 0) system("curl -X POST 'https://api.twilio.com/2010-04-01/Accounts/AC551d9951fe5ff6550be128a5c2c09468/Messages.json' --data-urlencode 'To=3146778934'  --data-urlencode 'From=+13142874589'  --data-urlencode 'Body=A spot may be open!' -u AC551d9951fe5ff6550be128a5c2c09468:550d74c1466512505650e3dce21c3771"); else printf("Class closed!");


    //}
   // if (scan != NULL) printf("%s\n",scan);else printf("Scan was null!\n");

    return 0;
}
