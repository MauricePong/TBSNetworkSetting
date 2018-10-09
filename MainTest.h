#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ParseTS_Length.h"
#include "ParsePAT.h"
#include "ParsePMT.h"
#include "ParseSDT.h"
#include "ParseEIT.h"
#include "ParseNIT.h"
#include "Program.h"

extern int iProgramCount_g;
extern PROGRAM_INFO astProgramInfo_g[PROGRAM_MAX];

/*********************************************************************************************************************************
 * Function Name : ParseAllProgramPMT
 * Description   :  parse all program PMT table
 * Parameters    :
 *				pfTsFile -- the TS file descriptor
 *				iTsPosition -- the TS begin position
 *				iTsLength -- the TS length
 *				pstPAT_Info -- the PAT_INFO contain the PMT_PID
 *				iProgramCount -- the program count
 *				pstPMT_Info -- the PMT_INFO array all program PMT information put in it
 * Returns       : -1 - failure  0 - success
 *********************************************************************************************************************************/
int ParseAllProgramPMT(FILE *pfTsFile, int iTsPosition, int iTsLength, PAT_INFO *pstPAT_Info, int iProgramCount,
	PMT_INFO *pstPMT_Info);

/*********************************************************************************************************************************
 * Function Name : ParseAllEIT
 * Description   :  parse EIT table and print the EIT information (table id is 0x4e 0x50)
 * Parameters    :
 *				pfTsFile -- the TS file descriptor
 *				iTsPosition -- the TS begin position
 *				iTsLength -- the TS length
 *				uiTableIdExtension -- the table_id_extension (the program id which we want to know the EPG information)
 * Returns       : -1 - failure 0 - success
 *********************************************************************************************************************************/
int ParseAllEIT(FILE *pfTsFile, int iTsPosition, int iTsLength, unsigned int uiTableIdExtension);

/*********************************************************************************************************************************
 * Function Name : ParseTransportStream
 * Description   :  parse the transport stream,print the program information and save the audio and video
 * Parameters    :
 *				pfTsFile -- the TS file descriptor
 * Returns       : -1 - failure  >0 - success
 *********************************************************************************************************************************/
int ParseTransportStream(FILE *pfTsFile);

/*********************************************************************************************************************************
 * Function Name : mainParseTS
 * Description   :  the main function
 * Parameters    : void
 * Returns       : -1 - failure >0 - success
 *********************************************************************************************************************************/
int mainParseTS(void);