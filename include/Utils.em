
/****************************Copyright(c)*****************************************
*                         ����������о΢���ӿƼ�                                  
*                              ͨ�ſ���ҵ��                                       
*--------------------------------FileInfo-----------------------------------------
*@File Name: Utils.c
*@Last modified Date: 2014-7-23               //�������
*@Last Version:                                          //�汾��Ϣ
*@Cread By: HZF                                   //����
*@Modified by:                                         //�޸���
*@Modified Time:                                       //�޸�ʱ��
*@brief: for all the modules                           //����
**********************************************************************************
*@note 
* 
* 
* 
* 
* 
*********************************************************************************/


//�ļ�ͷע��

macro InsertFileHeader()
{
	//szMyName = getenv(MYNAME)
	

	hbuf = GetCurrentBuf()
	ProgEnvInfo=GetProgramEnvironmentInfo()
	Author=ProgEnvInfo.UserName
	
	LocalTime =GetSysTime(1)
	Year=LocalTime.Year
	Month=LocalTime.Month
	Day=LocalTime.Day

	szBufName=GetBufName(hbuf)
	Len=strlen(szBufName)
	FileName=""
	if(0!=Len)
	{
		cch=Len
		while("\\"!= szBufName[cch])
		{
			cch=cch-1
		}

		while(cch<Len)
		{
			cch=cch+1
			FileName= Cat(FileName,szBufName[cch])
			//Msg("@FileName@")
		}
	}

	ln=1
	
	InsBufLine(hbuf,ln,"/*********************************Copyright(c)************************************")
	ln=ln+1
	InsBufLine(hbuf,ln,"*                         ������о΢���ӿƼ����޹�˾                          ")
	ln=ln+1
	InsBufLine(hbuf,ln,"*                              ͨ�ſ�����ҵ��                                      ")
	ln=ln+1
	InsBufLine(hbuf,ln,"*--------------------------------FileInfo-----------------------------------------")
	ln=ln+1
	InsBufLine(hbuf,ln,"*\@File Name: @FileName@")
	ln=ln+1
	InsBufLine(hbuf,ln,"*\@Last modified Date: @Year@-@Month@-@Day@               //�������")
	ln=ln+1
	InsBufLine(hbuf,ln,"*\@Last Version:1.0.1                                     //�汾��Ϣ")
	ln=ln+1
	InsBufLine(hbuf,ln,"*\@Cread By: @Author@                                   //����")
	ln=ln+1
	InsBufLine(hbuf,ln,"*\@Modified by:                                         //�޸���")
	ln=ln+1
	InsBufLine(hbuf,ln,"*\@Modified Time:                                       //�޸�ʱ��")
	ln=ln+1
	InsBufLine(hbuf,ln,"*\@brief: for all the modules                           //����")
	ln=ln+1
	InsBufLine(hbuf,ln,"**********************************************************************************")
	ln=ln+1
	InsBufLine(hbuf,ln,"*\@note ")
	ln=ln+1
	InsBufLine(hbuf,ln,"* ")
	ln=ln+1
	InsBufLine(hbuf,ln,"* ")
	ln=ln+1
	InsBufLine(hbuf,ln,"* ")
	ln=ln+1
	InsBufLine(hbuf,ln,"* ")
	ln=ln+1
	InsBufLine(hbuf,ln,"* ")
	ln=ln+1
	InsBufLine(hbuf,ln,"*********************************************************************************/")
	ln=ln+1

	SetBufIns(hbuf,ln,0)
	Len=strLen(FileName)
	if(("h" == tolower(FileName[Len-1])) && ("."== FileName[Len-2]))
	{
		FileName = toupper(FileName)
		FileName[Len-2]="_"
		szDef ="__"
		szDef=Cat(szDef,FileName)
		szDef=Cat(szDef,"__")

		ProgEnvInfo = GetProgramEnvironmentInfo()
		Editor =ProgEnvInfo.UserName

		hwnd=GetCurrentWnd()
		ln=GetWndSelLnFirst(hwnd)
		LocalTime =GetSysTime(1)
		Year=LocalTime.Year
		Month=LocalTime.Month
		Day=LocalTime.Day
		Time=LocalTime.Time
		hbuf=GetCurrentBuf()
		InsBufLine(hbuf,ln,"#ifndef @szDef@")
		ln=ln+1
		InsBufLine(hbuf,ln,"#define @szDef@")
		ln=ln+1
		InsBufLine(hbuf,ln,"")
		ln=ln+1
		InsBufLine(hbuf,ln,"")
		ln=ln+1
		InsBufLine(hbuf,ln,"")
		ln=ln+1
		InsBufLine(hbuf,ln,"#endif  /*ifndef @szDef@.@Year@-@Month@-@Day@ @Time@ @Editor@*/")
		ln=ln+1
	}

	SaveBuf(hbuf)
}



/**********************************************************************************
*@FuncName :InsertComment			//��������
*@brief 
* ��ӵ���ע��
* 
*@Para[in] : 				//��������
*@Para[out] : 				//�������
*@return :					//����ֵ 
*@note :						//ע��
*********************************************************************************/
macro InsertComment()
{
	sz=Ask("Enter Comment:")
	if(sz!= "")
	CommentSz(sz)
}

macro CommentSz(sz)
{
	hbufCur = GetCurrentBuf()
	SetBufSelText(hbufCur,"//!@sz@")
}


/**********************************************************************************
*@FuncName :InsertFunctionHeader			//��������
*@brief 
* 
* 
*@Para[in] : 				//��������
*@Para[out] : 				//�������
*@return :					//����ֵ 
*@note :						//ע��
*********************************************************************************/
macro InsertFunctionHeader()
{
	hbuf=GetCurrentBuf()
	ln=GetBufLnCur(hbuf)
	FuncName = GetCurSymbol()

	InsBufLine(hbuf,ln,"/**********************************************************************************")
	ln=ln+1
	InsBufLine(hbuf,ln,"*\@FuncName :@FuncName@			//��������")
	ln=ln+1
	InsBufLine(hbuf,ln,"*\@brief ")
	ln=ln+1
	InsBufLine(hbuf,ln,"* ")
	ln=ln+1
	InsBufLine(hbuf,ln,"* ")
	ln=ln+1
	InsBufLine(hbuf,ln,"*\@Para[in] : 				//��������")
	ln=ln+1
	InsBufLine(hbuf,ln,"*\@Para[out] : 				//�������")
	ln=ln+1
	InsBufLine(hbuf,ln,"*\@return :					//����ֵ ")
	ln=ln+1
	InsBufLine(hbuf,ln,"*\@note :						//ע��")
	ln=ln+1
	InsBufLine(hbuf,ln,"*********************************************************************************/")
	ln=ln+1

	Savebuf(hbuf)
}

macro Changetolower()
{
	hbuf=GetCurrentBuf()
	context=GetBufSelText(hbuf)
	context=tolower(context)

	SetBufSelText(hbuf,"@context@")

}
