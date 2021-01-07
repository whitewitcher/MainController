#include "param.h"

vs32 elmo_startps=25000;
vs32 elmo_throw=91112;
vs32 elmo_stop=25576;

FlagTypeDef flag;
ErrorTypeDef ErrorList;

void param_Init(void)
{
	DJ_motor_Init();
	VESCInit();
    Flag_Init();
	ErrorList.head=creatError();
}

void Flag_Init(void)
{
	flag.led=normal;
    flag.Can2CtrlList_Enable=true;
    
}
/* 改变16位数据高低顺序 */
 void ChangeData(u8* buff2,u8* buff1)
 {
	u8 temp=0;
    temp= *buff1;
	*buff1 = *buff2;	 
	*buff2 = temp;
 }

ErrorNode* creatError(void){
	ErrorNode* head;
	head=malloc(sizeof(ErrorNode*));
	head->next=malloc(sizeof(ErrorNode*));
	(head->next)->next=NULL;
	return head;
}

void deleteError(ErrorTypeDef errorList, ErrorNode* nodedel){
	if(nodedel==NULL)
		return;
	else{
		ErrorNode* errortemp=nodedel->next;
		nodedel->next=errortemp->next;
		free(errortemp);
	}
	return;
}

void insertError(ErrorTypeDef errorList, uc16 errorcode){
	ErrorNode* errortemp=errorList.head;
	for(int i=0;i<=errorList.size;i++)
		errortemp=errortemp->next;	
	errortemp->code=errorcode;
	errortemp->next=malloc(sizeof(ErrorNode*));
	(errortemp->next)->next=NULL;
	errorList.size++;
}

ErrorNode* ErrorFind(ErrorTypeDef errorList, uc16 errorcode){
	ErrorNode* errortemp=errorList.head;
	for(int i=0;i<errorList.size;i++){
		if((errortemp->next)->code==errorcode)
			return errortemp;
		else 
			errortemp=errortemp->next;
	}
	return NULL;
}

