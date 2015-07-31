//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <stdio.h>
#include <math.h>
/////////////////////////
#include <exception>
#include <iostream>
#include <sysopen.h> //Table1->AppendRecord(ARRAYOFCONST(...)]

using std::cout;
using std::endl;

typedef char char50[50]; //长度为50的char
#include <dir.h>
/////////////////////////
#include "ChildWin.h"

#include <dateutils.hpp> //wgs
#include <StrUtils.hpp>
#include <ADODB.hpp> //TADOTable
#include <time.h> //time_t

//==============================================================================
#include "DM1.h"
#include "wgsga.h" //Genetic Algorithm

#include "wgsSCEUA.h" //SCEUA Algorithm

#include "MPSAmap.h" //MPSA map of results
#include "matrixinv.h" //Matrix Inverse
#include "wgs_specialfunc.h" //Metropolis Monte Carlo Markov Chain
//==============================================================================

//---------------------------------------------------------------------
#pragma resource "*.dfm"
//---------------------------------------------------------------------
__fastcall TMDIChild::TMDIChild(TComponent *Owner)
: TForm(Owner)
{
}
//---------------------------------------------------------------------
void __fastcall TMDIChild::FormClose(TObject *Sender, TCloseAction &Action)
{
	if(ADOTable0->Active) ADOTable0->Close();
	if(ADOQuery1->Active) ADOQuery1->Close();
	Action = caFree;
}
//---------------------------------------------------------------------
void __fastcall TMDIChild::FormResize(TObject *Sender)
{
	ControlResize();
}
//---------------------------------------------------------------------------

void __fastcall TMDIChild::FormCreate(TObject *Sender)
{

	ControlResize();
	//==============================================================================
	WideString wstrProviderAccess = "Microsoft.Jet.OLEDB.4.0";
	if(DM->ADOConnection1->Provider == wstrProviderAccess) //强烈注意：Access Database,对DateTime字段的处理
		strQuoteA = "#";
	else
		strQuoteA = "'";
	//==============================================================================
	//Database1->Params->Add("User_Name=sa");//Values["User_Name"]="wgs";
	//Database1->Params->Add("Password=wgssql");//Values["Password"]="wgs";
	//Database1->LoginPrompt=false;
	//Database1->Connected = true;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------



//---------------------------------------------------------------------------

void __fastcall TMDIChild::Insert1Click(TObject *Sender)
{
	DBGrid1->DataSource->DataSet->Insert();
}
//---------------------------------------------------------------------------

void __fastcall TMDIChild::Append1Click(TObject *Sender)
{
	DBGrid1->DataSource->DataSet->Append();
}
//---------------------------------------------------------------------------

void __fastcall TMDIChild::Delete1Click(TObject *Sender)
{
	switch(MessageDlg("删除该项记录吗?",mtConfirmation,TMsgDlgButtons() <<mbNo<< mbYes,0))
	{case mrYes:
	if(DBGrid1->DataSource->DataSet->RecordCount>0)
		DBGrid1->DataSource->DataSet->Delete();
	break;
	case mrNo:break;}
}
//---------------------------------------------------------------------------

void __fastcall TMDIChild::DeleteAll1Click(TObject *Sender)
{
	switch(MessageDlg("删除所有记录吗?",mtConfirmation,TMsgDlgButtons() << mbYes<<mbNo,0))
	{case mrYes:
	{ProgressBar1->Visible=true;
	ProgressBar1->Min = 0;
	ProgressBar1->Max = 100;
	int iRecordCount = DBGrid1->DataSource->DataSet->RecordCount;
	DBGrid1->DataSource->DataSet->First();
	for (int i = 0; i < iRecordCount; i++)//for (int i = ProgressBar1->Min; i < ProgressBar1->Max; i++)
	{ DBGrid1->DataSource->DataSet->Delete();
	DBGrid1->DataSource->DataSet->Next();
	ProgressBar1->Position = 100*i/iRecordCount;}
	ProgressBar1->Visible=false;
	}
	break;
	case mrNo:break;}
}
//---------------------------------------------------------------------------

void __fastcall TMDIChild::EmptyTable1Click(TObject *Sender)
{
	String strTable = ADOTable0->TableName;
	String strQuery = "DELETE FROM " + strTable;
	switch(MessageDlg("清除表中所有记录吗?",mtConfirmation,TMsgDlgButtons() << mbYes<<mbNo,0))
	{case mrYes:
	DataQuery(false,"wgsdhm",strTable,"gridid",strQuery);
	//DM->ADOTable0->Active = false;
	//DM->ADOTable0->DeleteRecords(arAllChapters);
	//-------------------------
	// Table1->EmptyTable(); 但是ADOTable0没有这个方法？？？？
	//-------------------------
	break;
	case mrNo:break;}
}
//---------------------------------------------------------------------------
void __fastcall TMDIChild::Import1Click(TObject *Sender)
{
	char* sFile;
	DM->OpenDialog->DefaultExt="txt";
	DM->OpenDialog->FileName="*.txt";
	DM->OpenDialog->Filter="Text Files(*.txt)|*.txt";
	DM->OpenDialog->InitialDir=Application->ExeName;
	DM->OpenDialog->Title="选择导入数据文件";
	DM->OpenDialog->Options.Clear();
	DM->OpenDialog->Options<<ofFileMustExist
		<<ofHideReadOnly
		<<ofNoChangeDir
		<<ofCreatePrompt;
	if(DM->OpenDialog->Execute())
	{
		sFile=DM->OpenDialog->FileName.c_str(); //AnsiString转为char
		ImportFile(sFile);}

}
//---------------------------------------------------------------------------

void __fastcall TMDIChild::ImportFile(char *txtFileName)
{//根据一个表,判断其字段个数,字段类型,然后导入文本数据
	int nFieldCount,FieldDataType,i;
	int k=0;
	FILE *fpdata;

	nFieldCount = DBGrid1->DataSource->DataSet->FieldCount;

	if(FileExists(txtFileName))
	{
		if((fpdata=fopen(txtFileName,"r"))==NULL)
		{
			fprintf(stderr,"Cannot open input file.\n");
			return;
		}
	}
	else
	{
		ShowMessage(AnsiString("Couldnot find the file:")+txtFileName+".");
		return;
	}

	//char* DataFormat = new char[nFieldCount*2];
	//fscanfFormat(nFieldCount,DataFormat); //产生nFieldCount个"%s",如"%s%s%s%s"

	char50* chdata = new char50[nFieldCount]; //生成nFieldCount个长度为50的char类型
	ProgressBar1->Visible = true;
	ProgressBar1->Min = 0;
	ProgressBar1->Max = 100;

	for(;!feof(fpdata);)
	{

		for(i = 0; i < nFieldCount; i++)
			fscanf(fpdata,"%s",&chdata[i]);

		DBGrid1->DataSource->DataSet->Append();

		for(i = 0; i < nFieldCount; i++)
		{ //判断字段数据类型,将读取的字符型(char)转为相应类型,写入表中
			FieldDataType = DBGrid1->DataSource->DataSet->Fields->Fields[i]->DataType;
			switch (FieldDataType)
			{
			case ftSmallint: //2 int
				DBGrid1->DataSource->DataSet->Fields->Fields[i]->AsInteger = StrToInt(chdata[i]);
				break;
			case ftInteger: //3 long 转为float
				DBGrid1->DataSource->DataSet->Fields->Fields[i]->AsFloat = StrToInt(chdata[i]);
				break;
			case ftBoolean: //5 boolean 转为整型
				DBGrid1->DataSource->DataSet->Fields->Fields[i]->AsInteger = StrToInt(chdata[i]);
				break;
			case ftFloat: //6 float
				DBGrid1->DataSource->DataSet->Fields->Fields[i]->AsFloat = StrToFloat(chdata[i]);
				break;
			case ftCurrency: //8:Currency 转为实型
				DBGrid1->DataSource->DataSet->Fields->Fields[i]->AsFloat = StrToFloat(chdata[i]);
				break;
			case ftDateTime: //11 DateTime 转为日期时间
				DBGrid1->DataSource->DataSet->Fields->Fields[i]->AsDateTime = StrToDateTime(chdata[i]);
				break;
			case ftAutoInc: //14 自动编号 TAutoIncField 转为float
				DBGrid1->DataSource->DataSet->Fields->Fields[i]->AsFloat = StrToInt(chdata[i]);
				break;
			case ftString: //24string
				DBGrid1->DataSource->DataSet->Fields->Fields[i]->AsString = chdata[i];
				break;
			default:
				DBGrid1->DataSource->DataSet->Fields->Fields[i]->AsString = chdata[i];
				break;
			}//switch

		}//for(i = 0; i < nFieldCount; i++)
		if(k > ProgressBar1->Max)
			ProgressBar1->Position = 100*(k - 100)/k;
		else
			ProgressBar1->Position=k;
		k++;

	}//for(;!feof(fpdata);)
	//DBGrid1->DataSource->DataSet->Last(); //问题：不需要执行此句 ，否则删除出错
	DBGrid1->DataSource->DataSet->Delete(); //问题：最后一条记录总是导入2次，所以删除一次
	Edit1->Text="导入"+IntToStr(k-1)+"条记录";
	ProgressBar1->Visible = false;
	fclose(fpdata);
} //importfile

void* TMDIChild::fscanfFormat(int nFieldCount,char* DataFormat)
{//char* DataFormat = new char[nFieldCount*2];
	//例如:字段个数nFieldCount=5,函数结果 DataFormat="%s%s%s%s%s"
	for(int i=0;i<nFieldCount;i++)
	{DataFormat[i*2]='%';
	DataFormat[i*2+1]='s';}
	return DataFormat;
}



void __fastcall TMDIChild::ExportFile(TDataSet *DataSet,char *txtFileName)
{//根据一个表,判断其字段个数,字段类型,然后导出为文本数据
	int nFieldCount,nRecordCount,FieldDataType,i;
	int k=0;
	FILE *fpdata;

	nFieldCount = DataSet->FieldCount;

	if((fpdata=fopen(txtFileName,"w+"))==NULL)
	{
		fprintf(stderr,"Cannot open input file.\n");
		return;
	}
	//fprintf(fpdata,"%s","welcome") ;

	//char50* chdata = new char50[nFieldCount]; //生成nFieldCount个长度为50的char类型
	String* chdata = new String[nFieldCount];

	nRecordCount = DataSet->RecordCount;
	ProgressBar1->Visible=true;
	ProgressBar1->Min = 0;
	ProgressBar1->Max = 100;//nRecordCount;
	DataSet->First();
	for(k=0;k<nRecordCount;k++)
	{
		for(i = 0; i < nFieldCount; i++)
		{ //判断字段数据类型,将读取的字符型(char)转为相应类型,写入表中
			FieldDataType = DataSet->Fields->Fields[i]->DataType;
			switch (FieldDataType)
			{
			case ftSmallint: //int
				chdata[i]=IntToStr(DataSet->Fields->Fields[i]->AsInteger);
				break;
			case ftInteger: //long 转为float
				chdata[i]=IntToStr(DataSet->Fields->Fields[i]->AsInteger);
				break;
			case ftBoolean: //boolean 转为整型
				chdata[i]=BoolToStr(DataSet->Fields->Fields[i]->AsBoolean,true);
				break;
			case ftFloat: //float
				chdata[i]=FloatToStr(DataSet->Fields->Fields[i]->AsFloat);
				break;
			case ftCurrency: //Currency 转为实型
				chdata[i]=CurrToStr(DataSet->Fields->Fields[i]->AsCurrency);
				break;
			case ftDateTime: //DateTime 转为日期时间
				chdata[i]=DateTimeToStr(DataSet->Fields->Fields[i]->AsDateTime);
				break;
			case ftAutoInc: //自动编号 TAutoIncField 转为float
				chdata[i]=IntToStr(DataSet->Fields->Fields[i]->AsInteger);
				break;
			case ftString: //string
				chdata[i]=DataSet->Fields->Fields[i]->AsString;
				break;
			default:
				chdata[i]=DataSet->Fields->Fields[i]->AsString;
				break;
			}//switch

			fprintf(fpdata,"%s",chdata[i]);
			fprintf(fpdata,"\t"); //if(i<nFieldCount - 1)
		}//for(i = 0; i < nFieldCount; i++)
		fprintf(fpdata, "\n");
		ProgressBar1->Position=100.0*k/nRecordCount;
		DataSet->Next();

	}//for(k=0;k<DataSet->RecordCount;k++)
	ProgressBar1->Visible = false;
	Edit1->Text="共存储"+IntToStr(k)+"条记录";
	fclose(fpdata);
}//TMDIChild::ExportFile

void __fastcall TMDIChild::Export1Click(TObject *Sender)
{
	char* sFile;
	char szdir[MAXDIR];
	char szFileName[MAXFILE+4];
	int iFileHandle;
	TDataSet *DataSet;
	DataSet = DBGrid1->DataSource->DataSet;

	DM->SaveDialog->DefaultExt="txt";
	DM->SaveDialog->FileName="*.txt";
	DM->SaveDialog->Filter="Text Files(*.txt)|*.txt";
	DM->SaveDialog->InitialDir=Application->ExeName;
	DM->SaveDialog->Title="选择导出数据文件名";
	DM->SaveDialog->Options.Clear();
	DM->SaveDialog->Options<<ofFileMustExist
		<<ofHideReadOnly
		<<ofNoChangeDir;
	if(DM->SaveDialog->Execute())
	{
		if(FileExists(DM->SaveDialog->FileName))
		{
			ShowMessage(AnsiString("文件:")+DM->SaveDialog->FileName+"已经存在,扩展名已改为BAK.");
			fnsplit(DM->SaveDialog->FileName.c_str(), 0, szdir, szFileName, 0);
			strcat(szFileName, ".BAK");
			strcat(szdir,szFileName); //备份到原目录下
			RenameFile(DM->SaveDialog->FileName, szdir);
		}
		iFileHandle = FileCreate(DM->SaveDialog->FileName);
		FileClose(iFileHandle);
		sFile=DM->SaveDialog->FileName.c_str(); //AnsiString转为char
		ExportFile(DataSet,sFile);
	}

}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

void __fastcall TMDIChild::ControlResize()
{
	DBGrid1->Width = this->Width - 20 ;
	DBGrid1->Height = this->Height - 55 ;

	DBNavigator1->Top = DBGrid1->Top + DBGrid1->Height;
	ProgressBar1->Top = DBGrid1->Top + DBGrid1->Height;
	Edit1->Top = DBGrid1->Top + DBGrid1->Height + 5;
	Edit1->Width = DBGrid1->Width - DBNavigator1->Width - 10;

}

//------------------------------------------------------------------------

void __fastcall TMDIChild::RunDTVGM_Day()
{
	//ReadArrayofBagrovN(ArrayofBagrovN);
	//ShowMessage((String)ETatoETp(2.0,0.2));
	ShowMessage((String)(5/6));

}

//--------------------------------------------------------------------------------
int __fastcall TMDIChild::DataQuery(bool blOption,String strAlias,String strTable,String strField,String strQuery)
{//查询表BagrovN，并读取字段BagrovN的值
	//blOption = true,DataSource1->DataSet = Query1;
	//ShowMessage(strQuery);
	int NumofArray;
	this->Caption = strTable;
	Screen->Cursor=crHourGlass;
	if(ADOQuery1->Active) ADOQuery1->Close();
	//Query1->DatabaseName=strAlias;
	if(blOption==true) //DBGrid1显示ADOQuery1的内容
		DataSource1->DataSet = ADOQuery1;
	else
		DataSource1->DataSet = ADOTable0;
	DBGrid1->DataSource = DataSource1;
	//PopupMenu1->AutoPopup = false;
	PopupMenu1->Items->Items[0]->Enabled = false;
	PopupMenu1->Items->Items[1]->Enabled = false;
	PopupMenu1->Items->Items[3]->Enabled = false;
	PopupMenu1->Items->Items[4]->Enabled = false;
	PopupMenu1->Items->Items[5]->Enabled = false;
	PopupMenu1->Items->Items[7]->Enabled = false;

	ADOQuery1->SQL->Clear();
	ADOQuery1->SQL->Add(strQuery);
	ADOQuery1->Prepared; //大大加快查询速度
	if(LowerCase(LeftStr(strQuery,6)) == "select")
		ADOQuery1->Active=true;
	else
		ADOQuery1->ExecSQL();

	Screen->Cursor=crDefault;

	//if(DM->ADOQuery1->IsEmpty())
	// ShowMessage("Incorrect SQL Sentences\n OR NoRecords matched your criteria");

	if(ADOQuery1->Active)
	{ NumofArray = ADOQuery1->RecordCount;
	// ShowMessage(String(ADOQuery1->Fields->Fields[0]->AsString));
	if(NumofArray<1)
		Edit1->Text =strTable + ": No Records matched your criteria, Pls try again.";
	if(strField=="")
		Edit1->Text=strTable + "("+(String)ADOQuery1->RecordCount+"): "+"Now Showing all records...";
	else
		Edit1->Text=strTable + "("+(String)ADOQuery1->RecordCount+"): "+strQuery;
	}

	return NumofArray;
}//TMainForm::RunQuery

int __fastcall TMDIChild::SetFilter(bool blOption,String strAlias,String strTable,String strField,String strFilter)
{
	int NumofArray;
	this->Caption = strTable;
	Screen->Cursor=crHourGlass;
	if(ADOTable0->Active) ADOTable0->Close();
	//Table1->DatabaseName=strAlias;
	ADOTable0->TableName = strTable;
	if(blOption==true) //DBGrid1显示ADOTable0的内容
		DataSource1->DataSet = ADOTable0;
	else
		DataSource1->DataSet = ADOQuery1;
	DBGrid1->DataSource = DataSource1;

	PopupMenu1->AutoPopup = true;
	for (int i = 0; i < PopupMenu1->Items->Count; i++)
	{
		PopupMenu1->Items->Items[i]->Enabled = true;
	}

	ADOTable0->Filter = strFilter;
	ADOTable0->Filtered = true;

	ADOTable0->Active=true;
	Screen->Cursor=crDefault;

	if(ADOTable0->Active)
	{NumofArray = ADOTable0->RecordCount;
	if(NumofArray<1)
		Edit1->Text =strTable + ": No Records matched your criteria, Pls try again.";
	//throw Exception("No Records matched your criteria, Pls try again.");
	if(strField=="")
		Edit1->Text=strTable + "("+(String)ADOTable0->RecordCount+"): "+"Now Showing all records...";
	else
		Edit1->Text=strTable + "("+(String)ADOTable0->RecordCount+"): "+"Filter= '"+strFilter+"'";
	// Child->Show();
	}
	return NumofArray;
}//end SetFilter

void __fastcall TMDIChild::GaugeRain_DayToMonth()
{
	String strAlias,strTable,strField,strQuery,strFilter,strSumFields;
	strAlias = DM->ADOConnection1->DefaultDatabase;
	strTable = "dailyrain0";
	strField = "dt";
	TDate dateBegin,dateEnd,date1;
	int iyear,imonth,iday,iMonths,iDays,iGaugeCount;

	strQuery = "Select * FROM " + strTable + " WHERE "
		+ strField + " BETWEEN " + strQuoteA + DateToStr(gdateBegin) + strQuoteA
		+ " AND " + strQuoteA + DateToStr(gdateEnd) + strQuoteA + " ORDER BY DT";
	DataQuery(true,strAlias,strTable,strField,strQuery);

	if(ADOQuery1->RecordCount<1)
	{ShowMessage("No Records matched your criteria, Pls try again.");return;}
	ADOQuery1->First();
	dateBegin = ADOQuery1->FieldValues["dt"].date;//取查询出来的最早日期 //StrToDate((String)YearOf(gdateBegin)+"-"+(String)MonthOf(gdateBegin)+"-1");
	ADOQuery1->Last();
	dateEnd = ADOQuery1->FieldValues["dt"].date; //取查询出来的最晚日期 //StrToDate((String)YearOf(gdateEnd)+"-"+(String)MonthOf(gdateEnd)+"-1");
	iMonths = wgsMonthsBetween(dateBegin,dateEnd) ;//计算月数，MonthsBetween(2003-1-1,2004-1-1) = 11,实际上包括头尾有13个月
	//ShowMessage(IntToStr(iMonths));

	iGaugeCount = ADOQuery1->FieldCount - 1;
	for(int i = 1; i < iGaugeCount; i++)
		strSumFields = strSumFields + "SUM(" + ADOQuery1->Fields->Fields[i]->FieldName + "), ";
	strSumFields = strSumFields + "SUM(" + ADOQuery1->Fields->Fields[iGaugeCount]->FieldName + ")";

	ProgressBar1->Visible=true;
	ProgressBar1->Min = 0;
	ProgressBar1->Max = 100;
	iyear = YearOf(dateBegin);
	imonth = MonthOf(dateBegin) - 1;
	for(int i=0;i<iMonths;i++)//处理iMonth个月的数据
	{ imonth = imonth + 1;
	if(imonth>12)
	{iyear = iyear + 1;imonth = 1;}
	iDays = DaysInAMonth(iyear,imonth);
	dateBegin = StrToDate((String)iyear + "-" + (String)imonth + "-1");
	dateEnd = StrToDate((String)iyear + "-" + (String)imonth + "-" + (String)iDays);
	strQuery = "Select " + strSumFields +" FROM " + strTable + " WHERE "
		+ strField + " BETWEEN " + strQuoteA + DateToStr(dateBegin) + strQuoteA
		+ " AND " + strQuoteA + DateToStr(dateEnd) + strQuoteA;
	//ShowMessage(strQuery);
	DataQuery(false,strAlias,strTable,strField,strQuery);
	for(int j=0;j<iGaugeCount;j++)
	{strFilter = "DYear=" + (String)iyear + " AND DMonth=" + (String)imonth + " AND gaugeid=" + (String)(j+1);
	SetFilter(true,strAlias,"MonGageRain","DYear",strFilter);
	if(ADOTable0->RecordCount==1)
	{ADOTable0->Edit();
	ADOTable0->FieldValues["gaugerain"] = ADOQuery1->Fields->Fields[j]->AsFloat;}
	else if(ADOTable0->RecordCount==0)
	{ADOTable0->Append();
	ADOTable0->FieldValues["DYear"] = iyear;
	ADOTable0->FieldValues["DMonth"] = imonth;
	ADOTable0->FieldValues["gaugeid"] = j + 1;
	ADOTable0->FieldValues["gaugerain"] = ADOQuery1->Fields->Fields[j]->AsFloat;}
	ADOTable0->Post();
	} //j
	ProgressBar1->Position=100.0*i/iMonths;
	}//i
	ProgressBar1->Visible = false;
	if(YearOf(gdateBegin)==YearOf(gdateEnd))
		strFilter = "(DYear = " + (String)YearOf(gdateBegin) + " AND (DMonth >= " + (String)MonthOf(gdateBegin) + " AND DMonth <= " + (String)MonthOf(gdateEnd) + "))";
	else
		strFilter = "(DYear = " + (String)YearOf(gdateBegin) + " AND DMonth >= " + (String)MonthOf(gdateBegin) +
		") OR (DYear = " + (String)YearOf(gdateEnd) + " AND DMonth <= " + (String)MonthOf(gdateEnd) +
		") OR (DYear > " + (String)YearOf(gdateBegin) + " AND DYear < " + (String)YearOf(gdateEnd) + ")";
	SetFilter(true,strAlias,"MonGageRain","DYear",strFilter);
	ADOTable0->Sort = "DYear ASC, DMonth ASC, gaugeid ASC";
}//End TMDIChild::GaugeRain_DayToMonth

//---------------------------------------------------------------------------

void __fastcall TMDIChild::SubbFlowIn()
{//get the FlowIn Subbasins of a Subbasin
	String strAlias,strTable,strField,strQuery,strFilter;

	int i,nRows,iFlowTo;
	AnsiString asSubb;
	strAlias = DM->ADOConnection1->DefaultDatabase;
	strTable = "Streams";//"FlowRelation";
	strField = "subbasin";
	strFilter = strField + " > 0";
	nRows = SetFilter(true,strAlias,strTable,strField,strFilter);
	ADOTable0->Sort = strField + " ASC";

	AnsiString *asFlowIn = new AnsiString[nRows + 1];
	for(i = 0; i<nRows + 1; i++)
		asFlowIn[i] = "";

	ADOTable0->First();
	for(i = 0; i<nRows; i++)
	{
		asSubb = IntToStrF(ADOTable0->FieldValues["subbasin"],5);
		iFlowTo = ADOTable0->FieldValues["Subbasinr"]; //FlowTo
		asFlowIn[iFlowTo] = asFlowIn[iFlowTo] + asSubb;
		ADOTable0->Next();
	}

	ADOTable0->First();
	for(i = 0; i<nRows; i++)
	{
		ADOTable0->Edit();
		ADOTable0->FieldValues["Sname"] = asFlowIn[i + 1]; // FlowIn
		ADOTable0->Next();
	}
	delete[] asFlowIn;
	ShowMessage("FlowRelation Analysis Finished");
}

//---------------------------------------------------------------------------

extern double __fastcall DistanceBetween(struct TFPoint FPoint1,struct TFPoint FPoint2,char Algorithm)
{
	//char *Algorithm
	double fdistance;
	switch(Algorithm)
	{
	case 0: //欧氏距离平方
		fdistance = (FPoint1.x - FPoint2.x)*(FPoint1.x - FPoint2.x)
			+(FPoint1.y - FPoint2.y)*(FPoint1.y - FPoint2.y);
		break;
	case 1: //欧氏距离
		fdistance = (FPoint1.x - FPoint2.x)*(FPoint1.x - FPoint2.x)
			+(FPoint1.y - FPoint2.y)*(FPoint1.y - FPoint2.y);
		fdistance = sqrt(fdistance);
		break;
	default: fdistance = 0;break;
	}
	return fdistance;
} //End DistanceBetween

//---------------------------------------------------------------------------

extern void __fastcall RainIDW(int ngauge,int ngrid,double MinDistance,struct TFPoint FPointGauge[],struct TFPoint FPointGrid[],double gaugerain[],double gridrain[])
{//Rain Interpolation by IDW
	double fdistance;
	for(int i=0;i<ngrid;i++)
	{
		double drain=0,dsum=0;
		for(int j=0;j<ngauge;j++)
		{
			fdistance = DistanceBetween(FPointGrid[i],FPointGauge[j],0);
			if(fdistance<=MinDistance)//距离在一个grid内，用该雨量站雨量代表
			{gridrain[i] = gaugerain[j];goto Labelgoto;}
			else
			{drain = drain + gaugerain[j]/fdistance;
			dsum = dsum + 1.0/fdistance;}
		}//j
		gridrain[i] = drain/dsum;
Labelgoto:
	}//i

} //End RainIDW

//---------------------------------------------------------------------------
extern TDate __fastcall DateConsistOf(int iyear,int imonth,int iday)
{
	TDate DateConsist;
	DateConsist = StrToDate(IntToStr(iyear) + "-" + IntToStr(imonth) + "-" + IntToStr(iday));
	return DateConsist;
}
//---------------------------------------------------------------------------

void __fastcall TMDIChild::GridRain_IDW()
{//计算选定时间段的网格雨量
	AnsiString asFile;
	char *txtFileName;
	FILE *stream;
	int iFileHandle;
	const double ETpdef = 999.0;
	String strAlias,strTable,strField,strQuery,strFilter,strSumFields;
	strAlias = DM->ADOConnection1->DefaultDatabase;
	strTable = "raingauge";
	strField = "gaugeid";
	TDate dateBegin,dateEnd;
	int iyear,imonth,iday,iMonths;
	int i,j,k,ngauge,ngrid;
	int igridsize;
	String strDir;
	strQuery = "SELECT gridsize,dirrain FROM WorkDir WHERE gridname = '" + gstrDir + "'";
	DataQuery(false,strAlias,strTable,strField,strQuery);
	igridsize = ADOQuery1->FieldValues["gridsize"];
	strDir = ADOQuery1->FieldValues["dirrain"];
	double MinDistance = 2*Power(0.5*igridsize,2) ;//如栅格单元边长igridsize = 500
	//ShowMessage(strDir);
	strQuery = "Select * FROM " + strTable + " ORDER BY " + strField;
	DataQuery(true,strAlias,strTable,strField,strQuery);

	if(ADOQuery1->RecordCount<1)
	{ShowMessage("No Records matched your criteria, Pls try again.");return;}
	//else
	ProgressBar1->Visible=true;
	ProgressBar1->Min = 0;
	ProgressBar1->Max = 100;
	Edit1->Text = "Read Coordinates of RainGauges";
	// {
	ngauge = ADOQuery1->RecordCount;
	struct TFPoint *FPointGauge = new struct TFPoint[ngauge];
	double *gaugerain = new double[ngauge];
	ADOQuery1->First();
	for(j=0;j<ngauge;j++)//read gauge coordination
	{
		FPointGauge[j].x = ADOQuery1->FieldValues["coordx"];
		FPointGauge[j].y = ADOQuery1->FieldValues["coordy"];
		ADOQuery1->Next();
		ProgressBar1->Position=100.0*j/ngauge;
	}
	// }


	strTable = "SubBasins";
	strQuery = "Select coordx,coordy FROM " + strTable + " ORDER BY subbasin";
	DataQuery(false,strAlias,strTable,strField,strQuery);
	if(ADOQuery1->RecordCount<1)
	{ShowMessage("No Records matched your criteria, Pls try again.");return;}
	//else
	Edit1->Text = "Read Coordinates of Grids";
	// {
	ngrid = ADOQuery1->RecordCount;

	struct TFPoint *FPointGrid = new struct TFPoint[ngrid];
	double *gridrain = new double[ngrid];
	ADOQuery1->First();
	for(i=0;i<ngrid;i++)//read grid coordination
	{
		FPointGrid[i].x = ADOQuery1->FieldValues["coordx"];
		FPointGrid[i].y = ADOQuery1->FieldValues["coordy"];
		ADOQuery1->Next();
		ProgressBar1->Position=100.0*i/ngrid;
	}
	// }


	strTable = "MonGageRain"; //monthly rain of raingauges
	if(YearOf(gdateBegin)==YearOf(gdateEnd))
		strQuery = "Select * FROM " + strTable + " WHERE " +
		"(DYear = " + (String)YearOf(gdateBegin) + " AND (DMonth BETWEEN " + (String)MonthOf(gdateBegin) + " AND " + (String)MonthOf(gdateEnd) + "))";
	else
		strQuery = "Select * FROM " + strTable + " WHERE " +
		"(DYear = " + (String)YearOf(gdateBegin) + " AND DMonth >= " + (String)MonthOf(gdateBegin) +
		") OR (DYear = " + (String)YearOf(gdateEnd) + " AND DMonth <= " + (String)MonthOf(gdateEnd) +
		") OR (DYear > " + (String)YearOf(gdateBegin) + " AND DYear < " + (String)YearOf(gdateEnd) + ")";

	DataQuery(false,strAlias,strTable,strField,strQuery);

	if(ADOQuery1->RecordCount<1)
	{ShowMessage("No Records matched your criteria, Pls try again.");return;}
	ADOQuery1->First();
	dateBegin = DateConsistOf(ADOQuery1->FieldValues["DYear"],ADOQuery1->FieldValues["DMonth"],1);//取查询出来的最早日期 //
	ADOQuery1->Last();
	dateEnd = DateConsistOf(ADOQuery1->FieldValues["DYear"],ADOQuery1->FieldValues["DMonth"],1);//取查询出来的最晚日期 //StrToDate((String)YearOf(gdateEnd)+"-"+(String)MonthOf(gdateEnd)+"-1");
	iMonths = wgsMonthsBetween(dateBegin,dateEnd) ;//计算月数，MonthsBetween(2003-1-1,2004-1-1) = 11,实际上包括头尾有13个月
	//ShowMessage(IntToStr(iMonths));

	Hint = "Compute GridRain";

	iyear = YearOf(dateBegin);
	imonth = MonthOf(dateBegin) - 1;
	for(i=0;i<iMonths;i++)//处理iMonth个月的数据
	{
		imonth = imonth + 1;
		if(imonth>12)
		{iyear = iyear + 1;imonth = 1;}
		strQuery = "Select * FROM " + strTable + " WHERE " +
			"(DYear=" + (String)iyear + " AND DMonth=" + (String)imonth + ") ORDER by gaugeid";
		//ShowMessage(strQuery);
		DataQuery(false,strAlias,strTable,strField,strQuery);
		ADOQuery1->First();
		for(k=0;k<ngauge;k++)
		{gaugerain[k] = ADOQuery1->FieldValues["gaugerain"];ADOQuery1->Next();}
		//----------------------------------------------------------------------
		RainIDW(ngauge,ngrid,MinDistance,FPointGauge,FPointGrid,gaugerain,gridrain);//雨量插值
		//----------------------------------------------------------------------
		/*
		strFilter = "DYear=" + (String)iyear + " AND DMonth=" + (String)imonth;
		SetFilter(true,strAlias,"dbo.MonGridRain","DYear",strFilter);
		if(Table1->RecordCount>0)
		//－－－－－－－－－－－－－－－－－－－－－－有问题,效率太低，插入记录效率也很低
		{ShowMessage("Records Exists!!! Pls Delete!");
		return;
		//DeleteAllRecords(Table1);
		}
		//－－－－－－－－－－－－－－－－－－－－－－有问题 */
		//asFile = Trim(strDirRain + strPrefix_rainfile + IntToStr(iyear) + IntToStrF(imonth,2) + ".txt");
		asFile = strDir + "gridrain" + IntToStr(iyear) + IntToStrF(imonth,2) + ".txt";
		txtFileName = asFile.c_str();

		if(!FileExists(txtFileName))
		{iFileHandle = FileCreate(txtFileName);FileClose(iFileHandle);}
		if((stream = fopen(txtFileName, "w+"))==NULL)
		{
			ShowMessage("File Open Failed!!!");
			return;
		}
		for(j=0;j<ngrid;j++)
		{
			fprintf(stream,"%d\t %d\t %d\t %6.1f\t %6.1f",iyear,imonth,j+1,gridrain[j],ETpdef);
			fprintf(stream, "\n");
			//strQuery = "INSERT INTO dbo.MonGridRain(DYear,DMonth,gridid,gridrain) VALUES (" + IntToStr(iyear)+ "," +IntToStr(imonth)+ "," +IntToStr(j + 1)+ "," +FloatToStr(gridrain[j]) + ")";
			//DataQuery(false,strAlias,"dbo.MonGridRain",strField,strQuery); //SQL 语句效率也低，但是比Table->Append好一点
			//Table1->AppendRecord(ARRAYOFCONST((iyear,imonth,j+1,gridrain[j]))); //效率太低
			//ProgressBar1->Position=100.0*j/ngrid;
		}
		//Table1->Post();
		fclose(stream);
		ProgressBar1->Position=100.0*i/iMonths;
	}//i
	ProgressBar1->Visible = false;
	delete[] FPointGauge;
	delete[] FPointGrid;
	delete[] gaugerain;
	delete[] gridrain;
	/*strFilter = "(DYear=" + (String)YearOf(gdateBegin) + " AND DMonth>=" + (String)MonthOf(gdateBegin) +
	") OR (DYear=" + (String)YearOf(gdateEnd) + " AND DMonth<=" + (String)MonthOf(gdateEnd) +
	") OR (DYear>" + (String)YearOf(gdateBegin) + " AND DYear<" + (String)YearOf(gdateEnd) + ")";
	SetFilter(true,strAlias,"dbo.MonGageRain","DYear",strFilter); */
}//End TMDIChild::GridRain_IDW()

//---------------------------------------------------------------------------
void __fastcall TMDIChild::Discharge_DayToMonth()
{
	float fvar;
	String strAlias,strTable,strField,strQuery,strFilter,strStation;

	strAlias = DM->ADOConnection1->DefaultDatabase;
	strTable = "HydroMeteoStation";

	TDate dateBegin,dateEnd,date1;
	int iyear,imonth,iday,iMonths,iDays,nArray;

	strQuery = "Select station FROM " + strTable + " WHERE class = 'outlet'";
	nArray = DataQuery(true,strAlias,strTable,strField,strQuery); //ADOQuery1->RecordCount
	if(nArray<1)
	{ShowMessage("No Records matched your criteria, Pls try again.");return;}
	String *sStation = new String[nArray];
	ADOQuery1->First();
	for(int iStat=0; iStat < nArray; iStat++)
	{ sStation[iStat] = ADOQuery1->FieldValues["station"];
	ADOQuery1->Next();
	}

	strTable = "DayFlow";
	strField = "dt";

	for(int iStat = 0; iStat < nArray; iStat++)//Station: iStat
	{
		strStation = sStation[iStat];
		//==============================================================================

		strQuery = "Select dt FROM " + strTable + " WHERE ("
			+ strField + " BETWEEN " + strQuoteA + DateToStr(gdateBegin) + strQuoteA
			+ " AND " + strQuoteA + DateToStr(gdateEnd)+ strQuoteA
			+ ") AND station = '" + strStation
			+ "' ORDER BY dt";

		// strQuery = "Select dt FROM " + strTable + " WHERE ("
		// + strField + " BETWEEN '" + DateToStr(gdateBegin) + "'" + " AND '" + DateToStr(gdateEnd)+ "') AND station = '" + strStation + "'";
		//==============================================================================
		DataQuery(true,strAlias,strTable,strField,strQuery);

		if(ADOQuery1->RecordCount<1)
		{ShowMessage("No Records matched your criteria, Pls try again.");return;}
		ADOQuery1->First();
		dateBegin = ADOQuery1->FieldValues["dt"].date;//取查询出来的最早日期 //StrToDate((String)YearOf(gdateBegin)+"-"+(String)MonthOf(gdateBegin)+"-1");
		ADOQuery1->Last();
		dateEnd = ADOQuery1->FieldValues["dt"].date; //取查询出来的最晚日期 //StrToDate((String)YearOf(gdateEnd)+"-"+(String)MonthOf(gdateEnd)+"-1");
		iMonths = wgsMonthsBetween(dateBegin,dateEnd) ;//计算月数，MonthsBetween(2003-1-1,2004-1-1) = 11,实际上包括头尾有13个月
		//ShowMessage(IntToStr(iMonths));


		ProgressBar1->Visible=true;
		ProgressBar1->Min = 0;
		ProgressBar1->Max = 100;
		iyear = YearOf(dateBegin);
		imonth = MonthOf(dateBegin) - 1;
		for(int i=0;i<iMonths;i++)//处理iMonth个月的数据
		{ imonth = imonth + 1;
		if(imonth>12)
		{iyear = iyear + 1;imonth = 1;}
		iDays = DaysInAMonth(iyear,imonth);
		dateBegin = DateConsistOf(iyear,imonth,1);
		dateEnd = DateConsistOf(iyear,imonth,iDays);
		// strQuery = "Select AVG(qobs),AVG(qcal),SUM(pmean),AVG(wmean),SUM(ep),AVG(tempavg),MAX(tempmax),MIN(tempmin) FROM " + strTable
		strQuery = "Select SUM(qobs)/COUNT(qobs),SUM(qcal)/COUNT(qcal),SUM(pmean),SUM(wmean)/COUNT(wmean),SUM(ep),SUM(tempavg)/COUNT(tempavg),MAX(tempmax),MIN(tempmin) FROM " + strTable
			+ " WHERE station = '" + strStation +"' AND ("
			+ strField + " BETWEEN " + strQuoteA + DateToStr(dateBegin) + strQuoteA
			+ " AND " + strQuoteA + DateToStr(dateEnd) + strQuoteA + ")";
		//ShowMessage(strQuery);
		DataQuery(true,strAlias,strTable,strField,strQuery);
		// fvar = ADOQuery1->Fields->Fields[0]->AsFloat;
		strFilter = "DYear = " + (String)iyear + " AND DMonth = " + (String)imonth + " AND station = '" + strStation +"'";
		SetFilter(true,strAlias,"MonFlow","DYear",strFilter);
		if(ADOTable0->RecordCount==1)
		{ADOTable0->Edit();
		for(int j=0;j<8;j++){
			ADOTable0->Fields->Fields[j + 3]->AsFloat = ADOQuery1->Fields->Fields[j]->AsFloat;}}
		else if(ADOTable0->RecordCount==0)
		{ADOTable0->Append();
		ADOTable0->FieldValues["station"] = strStation;
		ADOTable0->FieldValues["DYear"] = iyear;
		ADOTable0->FieldValues["DMonth"] = imonth;
		for(int j=0;j<8;j++)
		{ fvar = ADOQuery1->Fields->Fields[j]->AsFloat;
		ADOTable0->Fields->Fields[j + 3]->AsFloat = fvar;
		}
		} //end else if
		ADOTable0->Post();

		ProgressBar1->Position=100.0*i/iMonths;
		}//i
	} // for(int iStat = 0; iStat < nArray; iStat++) //Station: iStat
	delete[] sStation;

	ProgressBar1->Visible = false;
	if(YearOf(gdateBegin)==YearOf(gdateEnd))
		strFilter = "station = '"+strStation+"' AND (DYear = " + (String)YearOf(gdateBegin) + " AND (DMonth >= " + (String)MonthOf(gdateBegin) + " AND DMonth <= " + (String)MonthOf(gdateEnd) + "))";
	else
		strFilter = "(station = '" + strStation + "' AND DYear = " + (String)YearOf(gdateBegin) + " AND DMonth >= " + (String)MonthOf(gdateBegin) +
		") OR (station = '" + strStation + "' AND DYear = " + (String)YearOf(gdateEnd) + " AND DMonth <= " + (String)MonthOf(gdateEnd) +
		") OR (station = '" + strStation + "' AND DYear > " + (String)YearOf(gdateBegin) + " AND DYear < " + (String)YearOf(gdateEnd) +")";
	SetFilter(true,strAlias,"MonFlow","DYear",strFilter);
}//End Discharge_DayToMonth()

//---------------------------------------------------------------------------

/******************************************************
* Function: DTVGM log File. *
* Variable: Model Parameters *
* Note: For different problem,user must change these *
******************************************************/
extern void __fastcall DTVGMlog(TADOTable *ADOTable,String SaveFile,double WaterBalance,double NashSutcliffeEC,
								double AW0,double AWC,double g1,double g2,double Kr,
								double PDc,double PDc_basic,double KAW,double KET,TDate DayStart,TDate DayEnd,
								TDateTime RunStart,TDateTime RunEnd,String SpendTime)
{

	if(ADOTable->Active)
		ADOTable->Close();

	ADOTable->Connection = DM->ADOConnection1;
	ADOTable->Active = false;
	ADOTable->TableName = "DTVGMlog";

	ADOTable->Open();

	ADOTable->Append();
	ADOTable->FieldValues["SaveFile"] = SaveFile;
	ADOTable->FieldValues["WaterBalance"] = WaterBalance;
	ADOTable->FieldValues["NashSutcliffeEC"] = NashSutcliffeEC;
	ADOTable->FieldValues["AW0"] = AW0;
	ADOTable->FieldValues["AWC"] = AWC;
	ADOTable->FieldValues["g1"] = g1;
	ADOTable->FieldValues["g2"] = g2;
	ADOTable->FieldValues["Kr"] = Kr;
	ADOTable->FieldValues["PDc"] = PDc;
	ADOTable->FieldValues["PDc_basic"] = PDc_basic;
	ADOTable->FieldValues["KAW"] = KAW;
	ADOTable->FieldValues["KET"] = KET;
	ADOTable->FieldValues["DayStart"] = DayStart;
	ADOTable->FieldValues["DayEnd"] = DayEnd;
	ADOTable->FieldValues["RunStart"] = RunStart;
	ADOTable->FieldValues["RunEnd"] = RunEnd;
	ADOTable->FieldValues["SpendTime"] = SpendTime;
	ADOTable->Post();
}

//---------------------------------------------------------------------------
/******************************************************
* Function: To calculate object value. *
* Variable: None *
* Note: For different problem,user must change these *
* code. *
******************************************************/

extern void __fastcall EvaluatePopulation(int nBasinGrid,int ngrid,TDate dateBegin,TDate dateEnd,
										  String strDirRain,String strPrefix_rainfile,
										  int nBagrovN,double *ArrayofBagrovN,double *gridBagrovN,
										  int *gridSubID,double *gridArea,double *gridRCC,
										  double *Array_ETp,double *RunoffObs,
										  double *Parameter,double *Parameter_optimize,TProgressBar *ProgressBar,
										  double *Mean_gridrain,double *Mean_gridRunoff,double *Mean_gridSurfaceRunoff,double *Mean_gridSubsurfaceRunoff,
										  double *Mean_wateruse,double *Mean_gridETa,double *Mean_gridsm,double *RunoffCal,double &WB,double &NSEC)
{
	int i,j;
	long temp;

	for (i=0;i<POPSIZE;i++)
	{
		for(j=0;j<NUMV;j++)
		{
			temp = DecodeChromosome(population[i].chrom,j*LENGTH,LENGTH);
			Parameter_optimize[j] = VariableValue(temp,Variable_min[j],Variable_max[j]);
		} //j

		population[i].value=DTVGM_Month(nBasinGrid,ngrid,dateBegin,dateEnd,
			strDirRain,strPrefix_rainfile,
			nBagrovN,ArrayofBagrovN,gridBagrovN,
			gridSubID,gridArea,gridRCC,
			Array_ETp,RunoffObs,
			Parameter,Parameter_optimize,ProgressBar,
			Mean_gridrain,Mean_gridRunoff,Mean_gridSurfaceRunoff,Mean_gridSubsurfaceRunoff,
			Mean_wateruse,Mean_gridETa,Mean_gridsm,RunoffCal,WB,NSEC);

	} //i
	CalculateFitnessValue( ); /* calculate fitness value */
	FindBestAndWorstIndividual( ); /* find the best and worst individual */
}

//---------------------------------------------------------------------------
void __fastcall TMDIChild::ReadArrayofBagrovN(double ArrayofBagrovN[])//
{//读取表 BagrovN 中的 N 值数组，作为插值的参照
	//CreateMDIChild("dbo.BagrovN");
	int NumofArray;
	String strAlias,strTable,strField,strQuery;
	strAlias = DM->ADOConnection1->DefaultDatabase;
	strTable = "BagrovN";
	strField = "PtoETp";
	strQuery = "Select BagrovN FROM " + strTable + " WHERE " + strField + "=0.0";
	NumofArray = DataQuery(true,strAlias,strTable,strField,strQuery);
	if(ADOQuery1->Active)
	{
		ADOQuery1->First();
		for(int i=0;i<NumofArray;i++)
		{ArrayofBagrovN[i] = ADOQuery1->FieldValues["BagrovN"];
		ADOQuery1->Next();
		}
	}

}//TMainForm::ReadArrayofBagrovN


//---------------------------------------------------------------------------
void __fastcall TMDIChild::RunDTVGM_Month(int Func)
{ //input,output, and call module: DTVGM_Month
	AnsiString asFile,strtemp,asFinish;
	char *txtFileName;
	FILE *stream,*frec;

	String strAlias,strTable,strField,strQuery,strFilter;
	strAlias = DM->ADOConnection1->DefaultDatabase;
	strTable = "WorkDir";
	strField = "gridname";
	TDate dateBegin,dateEnd;
	TDateTime RunStart,RunEnd;
	time_t time_Start,time_End;
	int SpendTime; //seconds
	int NumofArray;
	long ltemp;

	int iyear,imonth,iyear0,imonth0,nMonths;
	int i,j,k,ngrid,nBasinGrid,iRegion;

	String strDirWork,strDirRain,strDirResult,strDirSm;
	String strParaEn,strPrefix_rainfile,strPrefix_resultfile,strStation;
	double AW0,AWC,g1,g2,Kr,KAW,PDc,PDc_basic,BasinArea,KET;
	double ETp,PtoETp,dlETatoETp,doublevalue,Weight_WB,VOBF; //Value of OBject Function
	double WB,NSEC;//&WB=1.0,&NSEC=1.0;
	float ftemp;
	double Parameter[6];
	//const int nopt = 5; //5个优化参数//再加一个参数aRCC,产流折算系数
	const int nopt = 5; //g1,g2,Kr,KAW
	int nrun; //运行次数
	double Parameter_optimize[nopt];
	// AnsiString asPName[] = {"g1","g2","Kr","KAW","AW0"};

	strStation = gstrBasinSelect;

	ProgressBar1->Visible = true;
	//-------------------------------------------------
	//Read Table WorkDir

	strQuery = "SELECT * FROM WorkDir WHERE gridname = '" + gstrDir + "'";
	DataQuery(false,strAlias,strTable,strField,strQuery);
	ADOQuery1->First();
	strDirWork = ADOQuery1->FieldValues["DirWork"];
	strDirRain = ADOQuery1->FieldValues["DirRain"];
	strDirResult = ADOQuery1->FieldValues["DirResult"];
	strDirSm = ADOQuery1->FieldValues["DirSm"];
	//-------------------------------------------------
	//Read Table ModelParameters

	strQuery = "SELECT * FROM ModelParameters WHERE Scale = 'Month'";
	NumofArray = DataQuery(false,strAlias,strTable,strField,strQuery);
	ADOQuery1->First();
	for(i=0;i<NumofArray;i++)
	{
		strParaEn = ADOQuery1->FieldValues["ParaEn"];
		if(strParaEn == "aAW0")
		{AW0 = ADOQuery1->FieldValues["ParaValue"];
		Variable_min[4] = ADOQuery1->FieldValues["ParaMin"];
		Variable_max[4] = ADOQuery1->FieldValues["ParaMax"];}
		else if (strParaEn == "bAWC")
			AWC = ADOQuery1->FieldValues["ParaValue"];
		else if (strParaEn == "ag1")
		{g1 = ADOQuery1->FieldValues["ParaValue"];
		Variable_min[0] = ADOQuery1->FieldValues["ParaMin"];
		Variable_max[0] = ADOQuery1->FieldValues["ParaMax"];}
		else if (strParaEn == "ag2")
		{g2 = ADOQuery1->FieldValues["ParaValue"];
		Variable_min[1] = ADOQuery1->FieldValues["ParaMin"];
		Variable_max[1] = ADOQuery1->FieldValues["ParaMax"];}
		else if (strParaEn == "aKr")
		{Kr = ADOQuery1->FieldValues["ParaValue"];
		Variable_min[2] = ADOQuery1->FieldValues["ParaMin"];
		Variable_max[2] = ADOQuery1->FieldValues["ParaMax"];}
		else if(strParaEn == "aKAW")
		{KAW = ADOQuery1->FieldValues["ParaValue"];
		Variable_min[3] = ADOQuery1->FieldValues["ParaMin"];
		Variable_max[3] = ADOQuery1->FieldValues["ParaMax"];}
		else if (strParaEn == "bPDc")
			PDc = ADOQuery1->FieldValues["ParaValue"];
		else if(strParaEn == "bPDc_basic")
			PDc_basic = ADOQuery1->FieldValues["ParaValue"];
		else if(strParaEn == "cPrefix_r")
			strPrefix_rainfile = ADOQuery1->FieldValues["unit"];
		else if(strParaEn == "cPrefix_q")
			strPrefix_resultfile = ADOQuery1->FieldValues["unit"];
		else if(strParaEn == "cgridnum")
			nBasinGrid = ADOQuery1->FieldValues["ParaValue"]; //Number of the whole subbasins
		else if(strParaEn == "cBasinArea")
			BasinArea = ADOQuery1->FieldValues["ParaValue"];
		else if(strParaEn == "bKET")
			KET = ADOQuery1->FieldValues["ParaValue"];
		else if(strParaEn == "bWeight_WB")
			Weight_WB = ADOQuery1->FieldValues["ParaValue"];
		else if(strParaEn == "bMaxGen")
			MaxGeneration = ADOQuery1->FieldValues["ParaValue"];

		ADOQuery1->Next();
	}
	Parameter_optimize[0] = g1;
	Parameter_optimize[1] = g2;
	Parameter_optimize[2] = Kr;
	Parameter_optimize[3] = KAW;
	Parameter_optimize[4] = AW0;

	Parameter[0] = AWC;
	Parameter[1] = PDc;
	Parameter[2] = PDc_basic;
	Parameter[3] = KET;
	Parameter[4] = BasinArea;
	Parameter[5] = Weight_WB;
	//-------------------------------------------------
	strTable = "RegionCC";
	strQuery = "Select * FROM " + strTable + " ORDER by Region";
	iRegion = DataQuery(false,strAlias,strTable,strField,strQuery);
	if(iRegion<1)
	{ShowMessage("RegionCC: No Records matched your criteria, Pls try again.");return;}
	double *RegionCC = new double[iRegion];
	ADOQuery1->First();
	for(i = 0;i<iRegion;i++)
	{
		RegionCC[i] = ADOQuery1->FieldValues["RegionCC"]; //
		ADOQuery1->Next();
	}

	//Read Table "subbasins", obatin the BagrovN of each grid;

	strTable = "HydroMeteoStation";
	strQuery = "Select station,Nsubbasins FROM " + strTable + " WHERE station = '" + strStation+ "'";
	DataQuery(false,strAlias,strTable,strField,strQuery);
	if(ADOQuery1->RecordCount<1)
	{ShowMessage("No Records matched your criteria, Pls try again.");return;}
	ADOQuery1->First();
	ngrid = ADOQuery1->FieldValues["Nsubbasins"]; //Number of subbasins of selected Basin, such as Chaohe River or Baihe River

	double *gridBagrovN = new double[ngrid];
	int *gridSubID = new int[ngrid];// subbasin ID of Obasin in Wbasin;
	double *gridArea = new double[ngrid];//: Area of subbasin in Obasin
	double *gridRCC = new double[ngrid];//: Region Convert Coefficient of subbasin Runoff


	strTable = "SubBasins";
	strQuery = "Select subbasin,Area,BagrovN,Region FROM "
		+ strTable + " WHERE Outlet = '" + strStation
		+ "' ORDER BY subbasin";
	DataQuery(false,strAlias,strTable,strField,strQuery);
	if(ADOQuery1->RecordCount<1)
	{ShowMessage("SubBasins:No Records matched your criteria, Pls try again.");return;}
	ADOQuery1->First();
	for(k=0;k<ngrid;k++)
	{
		gridSubID[k] = ADOQuery1->FieldValues["subbasin"];
		gridArea[k] = ADOQuery1->FieldValues["Area"];
		gridBagrovN[k] = ADOQuery1->FieldValues["BagrovN"];
		iRegion = ADOQuery1->FieldValues["Region"];
		gridRCC[k] = RegionCC[iRegion - 1];
		ADOQuery1->Next();
		ProgressBar1->Position=100.0*k/ngrid;
	}
	delete[] RegionCC;
	//-------------------------------------------------

	/*//Read Table "HydroMeteoStation", obatin the station name of outlet;

	strTable = "HydroMeteoStation";
	strQuery = "Select station FROM " + strTable + " WHERE class = 'outlet'";
	DataQuery(false,strAlias,strTable,strField,strQuery);
	if(ADOQuery1->RecordCount<1)
	{ShowMessage("No Records matched your criteria, Pls try again.");return;}
	strStation = ADOQuery1->FieldValues["station"]; */

	//obtain the ARRAY of BagrovN
	strTable = "BagrovN";
	strField = "PtoETp";
	strQuery = "Select BagrovN FROM " + strTable + " WHERE " + strField + "=0.0 Order by BagrovN";
	int nBagrovN = DataQuery(true,strAlias,strTable,strField,strQuery);
	double *ArrayofBagrovN = new double[nBagrovN]; //
	ADOQuery1->First();
	for(int i=0;i<nBagrovN;i++)
	{ ArrayofBagrovN[i] = ADOQuery1->FieldValues["BagrovN"];
	ADOQuery1->Next();
	////ShowMessage(String(ArrayofBagrovN[i]));
	}
	////ReadArrayofBagrovN(ArrayofBagrovN); //obtain the ARRAY of BagrovN

	////ShowMessage(String(sizeof(ArrayofBagrovN))+":"+String(sizeof(ArrayofBagrovN[0])));

	//Read Table MonFlow, Judge whether there is enough data for simulation
	strTable = "MonFlow"; //monthly Discharge,ETp,Temperature,... 必须在计算之前读取该表，同时该表为计算实时提供ETp等数据
	if(YearOf(gdateBegin)==YearOf(gdateEnd))
		strFilter = "station = '"+strStation+"' AND (DYear = " + (String)YearOf(gdateBegin) + " AND (DMonth >= " + (String)MonthOf(gdateBegin) + " AND DMonth <= " + (String)MonthOf(gdateEnd) + "))";
	else
		strFilter = "(station = '" + strStation + "' AND DYear = " + (String)YearOf(gdateBegin) + " AND DMonth >= " + (String)MonthOf(gdateBegin) +
		") OR (station = '" + strStation + "' AND DYear = " + (String)YearOf(gdateEnd) + " AND DMonth <= " + (String)MonthOf(gdateEnd) +
		") OR (station = '" + strStation + "' AND DYear > " + (String)YearOf(gdateBegin) + " AND DYear < " + (String)YearOf(gdateEnd) +")";
	SetFilter(true,strAlias,strTable,"DYear",strFilter);

	if(ADOTable0->RecordCount<1)
	{ShowMessage("No Records matched your criteria, Pls try again.");return;}

	ADOTable0->Sort = "DYear ASC, DMonth ASC";
	ADOTable0->First();
	dateBegin = DateConsistOf(ADOTable0->FieldValues["DYear"],ADOTable0->FieldValues["DMonth"],1);//取查询出来的最早日期 //
	ADOTable0->Last();
	dateEnd = DateConsistOf(ADOTable0->FieldValues["DYear"],ADOTable0->FieldValues["DMonth"],1);//取查询出来的最晚日期 //StrToDate((String)YearOf(gdateEnd)+"-"+(String)MonthOf(gdateEnd)+"-1");
	nMonths = wgsMonthsBetween(dateBegin,dateEnd) ; //计算月数，MonthsBetween(2003-1-1,2004-1-1) = 11,实际上包括头尾有13个月

	double *RunoffObs = new double[nMonths];
	double *RunoffCal = new double[nMonths];
	double *RunoffCal0 = new double[nMonths];

	double *Array_ETp = new double[nMonths];
	double *Mean_gridrain = new double[nMonths];
	double *Mean_gridRunoff = new double[nMonths];
	double *Mean_gridSurfaceRunoff = new double[nMonths];
	double *Mean_gridSubsurfaceRunoff = new double[nMonths];
	double *Mean_gridETa = new double[nMonths];
	double *Mean_gridsm = new double[nMonths];
	double *Mean_wateruse = new double[nMonths];//: Mean of subbasin wateruse in Obasin

	ADOTable0->First();
	for(i=0;i<nMonths;i++)
	{
		RunoffObs[i] = ADOTable0->FieldValues["qobs"];
		Array_ETp[i] = ADOTable0->FieldValues["ep"];
		ADOTable0->Next();
	}
	//-------------------------------------------------
	RunStart = Now(); time_Start = time(NULL);

	//000000000000000000000000000000000000000000000000000000000000000000000000000000
	if(Func == 0)
	{//BEGIN:DTVGM Simulation

		VOBF = DTVGM_Month(nBasinGrid,ngrid,dateBegin,dateEnd,
			strDirRain,strPrefix_rainfile,
			nBagrovN,ArrayofBagrovN,gridBagrovN,
			gridSubID,gridArea,gridRCC,
			Array_ETp,RunoffObs,
			Parameter,Parameter_optimize,ProgressBar1,
			Mean_gridrain,Mean_gridRunoff,Mean_gridSurfaceRunoff,Mean_gridSubsurfaceRunoff,
			Mean_wateruse,Mean_gridETa,Mean_gridsm,RunoffCal,WB,NSEC);
                asFinish = "DTVGM Simulation Finished";
	}//END: DTVGM Simulation
	//11111111111111111111111111111111111111111111111111111111111111111111111111111
	else if(Func == 1)
	{//BEGIN: Run Genetic Algorithm
		asFile = strDirSm + "ga.out";
		if((fpga0=fopen(asFile.c_str(),"w+"))==NULL)//E:\\DTVGM_CB
		{
			ShowMessage("Cannot open input file.\n");
			return;
		}

		generation=0;
		GenerateInitialPopulation();
		EvaluatePopulation(nBasinGrid,ngrid,dateBegin,dateEnd,
			strDirRain,strPrefix_rainfile,
			nBagrovN,ArrayofBagrovN,gridBagrovN,
			gridSubID,gridArea,gridRCC,
			Array_ETp,RunoffObs,
			Parameter,Parameter_optimize,ProgressBar1,
			Mean_gridrain,Mean_gridRunoff,Mean_gridSurfaceRunoff,Mean_gridSubsurfaceRunoff,
			Mean_wateruse,Mean_gridETa,Mean_gridsm,RunoffCal,WB,NSEC); /* calculate object value */

		while (generation<MaxGeneration)
		{
			generation++;
			GenerateInitialPopulation();

			EvaluatePopulation(nBasinGrid,ngrid,dateBegin,dateEnd,
				strDirRain,strPrefix_rainfile,
				nBagrovN,ArrayofBagrovN,gridBagrovN,
				gridSubID,gridArea,gridRCC,
				Array_ETp,RunoffObs,
				Parameter,Parameter_optimize,ProgressBar1,
				Mean_gridrain,Mean_gridRunoff,Mean_gridSurfaceRunoff,Mean_gridSubsurfaceRunoff,
				Mean_wateruse,Mean_gridETa,Mean_gridsm,RunoffCal,WB,NSEC); /* calculate object value */

			PerformEvolution();
			OutputTextReport ();
		} //end while
		fclose(fpga0);
		g1 = Vx[0];
		g2 = Vx[1];
		Kr = Vx[2];
		KAW = Vx[3];
		AW0 = Vx[4];
		//Parameter_optimize = Vx;
		VOBF = DTVGM_Month(nBasinGrid,ngrid,dateBegin,dateEnd,
			strDirRain,strPrefix_rainfile,
			nBagrovN,ArrayofBagrovN,gridBagrovN,
			gridSubID,gridArea,gridRCC,
			Array_ETp,RunoffObs,
			Parameter,Vx,ProgressBar1,
			Mean_gridrain,Mean_gridRunoff,Mean_gridSurfaceRunoff,Mean_gridSubsurfaceRunoff,
			Mean_wateruse,Mean_gridETa,Mean_gridsm,RunoffCal,WB,NSEC);
                asFinish = "Genetic Algorithm Opimizing Finished";
	} //END: Run Genetic Algorithm

	//222222222222222222222222222222222222222222222222222222222222222222222222222222
	else if(Func == 2)
	{//BEGIN: Run SCEUA Algorithm

		asFile = strDirSm + "sceua.out";
		if((ipr=fopen(asFile.c_str(),"w+"))==NULL)//E:\\DTVGM_CB
		{
			ShowMessage("Cannot open input file.\n");
			return;
		}

		// const int nopt = 5; //5个优化参数

		int maxn,kstop,ngs,iseed;
		double pcento;
		int npg,nps,nspl,mings,iniflg,iprint;
		double a[nopt], bl[nopt], bu[nopt];
		//&maxn,&kstop,&pcento0,&ngs,&iseed,&ideflt = 10000 10.0001 2 10 0
		maxn = 10000;
		kstop = 10;
		pcento = .0001;
		ngs = 2;
		iseed = MaxGeneration; //10

		npg = 2*nopt + 1;
		nps = nopt + 1;
		nspl = npg;
		mings = ngs;
		iniflg = 0;
		iprint = 0;
		// pcntrl = deflt;

		for(i = 0 ;i < nopt;i++)
		{
			a[i] = Parameter_optimize[i];
			bl[i] = Variable_min[i];
			bu[i] = Variable_max[i];
		}

		// iff = 0;
		int jseed[] ={2,3,5,7,11,13,17,19,23,29};

//		ShowMessage(" >>> ENTER THE MAIN PROGRAM of SCE-UA<<< ");

		// FunctSet();
		// scein(a,bl,bu,nopt,maxn,kstop,pcento,iseed,ngs,npg,nps,nspl,mings,iniflg,iprint);
		if (iseed > 0)
			nrun = min(iseed,10);
		else
			nrun = 1;

		for(int i=0;i<nrun;i++)
		{
			if (nrun != 1) iseed = jseed[i];
			fprintf(ipr,"\n\n@ SCE-UA Run Number = %d \n Random Seed Value = %d \n\n",i,iseed);
			sceua(a,bl,bu,nopt,maxn,kstop,pcento,iseed,ngs,npg,nps,nspl,mings,iniflg,iprint,
				nBasinGrid,ngrid,dateBegin,dateEnd,
				strDirRain,strPrefix_rainfile,
				nBagrovN,ArrayofBagrovN,gridBagrovN,
				gridSubID,gridArea,gridRCC,
				Array_ETp,RunoffObs,
				Parameter,Parameter_optimize,ProgressBar1,
				Mean_gridrain,Mean_gridRunoff,Mean_gridSurfaceRunoff,Mean_gridSubsurfaceRunoff,
				Mean_wateruse,Mean_gridETa,Mean_gridsm,RunoffCal,WB,NSEC);
		}//end do i

		fclose(ipr);

		g1 = bestpara[0];
		g2 = bestpara[1];
		Kr = bestpara[2];
		KAW = bestpara[3];
		AW0 = AWC*0.2;//bestpara[4];
		//Parameter_optimize = Vx;
		VOBF = DTVGM_Month(nBasinGrid,ngrid,dateBegin,dateEnd,
			strDirRain,strPrefix_rainfile,
			nBagrovN,ArrayofBagrovN,gridBagrovN,
			gridSubID,gridArea,gridRCC,
			Array_ETp,RunoffObs,
			Parameter,bestpara,ProgressBar1,
			Mean_gridrain,Mean_gridRunoff,Mean_gridSurfaceRunoff,Mean_gridSubsurfaceRunoff,
			Mean_wateruse,Mean_gridETa,Mean_gridsm,RunoffCal,WB,NSEC);
                asFinish = "SCE-UA Algorithm Opimizing Finished";
	} //END: Run SCEUA Algorithm

	//333333333333333333333333333333333333333333333333333333333333333333333333333333
	else if(Func == 3)
	{//BEGIN: Run MPSA--Multiple Parameters Sensitivity Analysis
		nrun = MaxGeneration; //运行模型次数

		/* 不需要para[][]
		double **para;
		para = new double*[nrun]; // STEP 1: SET UP THE ROWS.
		for (i = 0; i < nrun; i++)
		para[i] = new double[nopt]; // STEP 2: SET UP THE COLUMNS
		*/

		asFile = strDirSm + "mpsa.out";
		if((stream=fopen(asFile.c_str(),"w+"))==NULL)//E:\\DTVGM_CB
		{
			ShowMessage("Cannot open input file.");
			return;
		}

		for(j=0;j<nopt;j++) //实测值通常是用每个参数取值范围的中值代入模型进行模拟得到
			Parameter_optimize[j] = 0.5*(Variable_min[j] + Variable_max[j]);

		VOBF = DTVGM_Month(nBasinGrid,ngrid,dateBegin,dateEnd,
			strDirRain,strPrefix_rainfile,
			nBagrovN,ArrayofBagrovN,gridBagrovN,
			gridSubID,gridArea,gridRCC,
			Array_ETp,RunoffObs,
			Parameter,Parameter_optimize,ProgressBar1,
			Mean_gridrain,Mean_gridRunoff,Mean_gridSurfaceRunoff,Mean_gridSubsurfaceRunoff,
			Mean_wateruse,Mean_gridETa,Mean_gridsm,RunoffCal0,WB,NSEC);


		AnsiString paraName[] = {"g1","g2","Kr","KAW","AW0"};
		double EVobs,EVmid,HMLEobs,HMLEmid;
		//double BoxTaio,EVexp,FuzzyAdd,FuzzyUnion,FuzzyInsec; //NSEC,

		fprintf(stream,"Basin: %s\n",strStation);
		fprintf(stream,"Model_Runs: %d\n",nrun);
		fprintf(stream,"ParaNumbers: %d\n",nopt);
		fprintf(stream,"VOBFNumbers: %d\n",nopt);
		for(j=0;j<nopt;j++)
			fprintf(stream,"%s%10.4f%10.4f\n",paraName[j],Variable_min[j],Variable_max[j]);

		fprintf(stream,"\nMonteCarlo-Simulation-Results\n");
		//fprintf(stream,"i\t\t g1\t\t\t\t g2\t\t\t\t Kr\t\t\t\t KAW \t\t\t\t AW0 \t\t\t\t VOBFr\n");
		fprintf(stream,"i\t\t g1\t\t\t\t g2\t\t\t\t Kr\t\t\t\t KAW \t\t\t\t AW0\t\t\t\t");
		fprintf(stream,"EVOBS\t\t\t\t\EVMID\t\t\t\tHMLEOBS\t\t\t\tHMLEMID\t\t\t\tNSEC\n");
		// fprintf(stream,"NSEC\t\t\t\t\BoxTaio\t\t\t\t\EVEXP\t\t\t\t\");
		// fprintf(stream,"FuzzyAdd\t\t\t\t\FuzzyUnion\t\t\t\t\FuzzyIntersection\n");

		for(i=0;i<nrun;i++)
		{ fprintf(stream,"%d",i + 1);
		for(j=0;j<nopt;j++)
		{
			Parameter_optimize[j] = Variable_min[j] + (Variable_max[j] - Variable_min[j])*random(1e4)/1.0e4;
			// para[i][j] = Parameter_optimize[j];
			fprintf(stream,"%10.4f",Parameter_optimize[j]);
		}



		DTVGM_Month(nBasinGrid,ngrid,dateBegin,dateEnd,
			strDirRain,strPrefix_rainfile,
			nBagrovN,ArrayofBagrovN,gridBagrovN,
			gridSubID,gridArea,gridRCC,
			Array_ETp,RunoffObs,
			Parameter,Parameter_optimize,ProgressBar1,
			Mean_gridrain,Mean_gridRunoff,Mean_gridSurfaceRunoff,Mean_gridSubsurfaceRunoff,
			Mean_wateruse,Mean_gridETa,Mean_gridsm,RunoffCal,WB,NSEC);

		EVobs = ErrorVariance(nMonths,RunoffObs,RunoffCal);
		EVmid = ErrorVariance(nMonths,RunoffCal0,RunoffCal);
		HMLEobs = HMLE(0,nMonths,RunoffObs,RunoffCal);
		HMLEmid = HMLE(0,nMonths,RunoffCal0,RunoffCal);

		//NSEC
		//BoxTaio,EVexp,FuzzyAdd,FuzzyUnion,FuzzyInsec; //可以由前面的生成
		//VOBFr[i] = ErrorVariance(nMonths,RunoffCal0,RunoffCal)
		fprintf(stream,"%14.4f%14.4f%14.4f%14.4f%14.4f\n",EVobs,EVmid,HMLEobs,HMLEmid,NSEC);
                asFinish = "Multiple Parameters Sensitivity Analysis Finished";
		}//end i
		// } //end j
		/*
		fprintf(stream,"\nMonteCarlo-Simulation-Sorting\n");
		fprintf(stream,"i\t\t g1\t\t\t\t g2\t\t\t\t Kr\t\t\t\t KAW \t\t\t\t AW0\t\t\t\t");
		fprintf(stream,"EVOBS\t\t\t\t\EVMID\t\t\t\tHMLEOBS\t\t\t\tHMLEMID\t\t\t\t");
		fprintf(stream,"NSEC\t\t\t\t\BoxTaio\t\t\t\t\EVEXP\t\t\t\t\");
		fprintf(stream,"FuzzyAdd\t\t\t\t\FuzzyUnion\t\t\t\t\FuzzyIntersection\n");
		sort(nrun,nopt,para,VOBFr);
		for(i=0;i<nrun;i++)
		{ fprintf(stream,"%d",i + 1);
		for(j=0;j<nopt;j++)
		{
		fprintf(stream,"%10.4f",para[i][j]);
		}
		fprintf(stream,"%20.4f\n",VOBFr[i]);
		} //end i

		fprintf(stream,"\n=================Calulate Cumulative Frequency==================\n");
		fprintf(stream,"k \t paraA \t CumuFreqA \t paraUA CumuFreqUA\n");
		int nrunA,nrunUA; //numbers of Acceptable and UnAcceptable
		nrunA = nrun/2;
		nrunUA = nrun - nrunA; //nrunUA = nrunA or nrunA + 1
		double *paraA = new double[nrunA]; //Acceptable
		double *paraUA = new double[nrunUA]; //UnAcceptable
		//-----------------------------------------------------


		for(j=0;j<nopt;j++)
		{ fprintf(stream,"\nBegin:Parameter[%d]---\n",j);

		for(k=0;k<nrunA;k++)
		paraA[k] = para[k][j];
		for(k=0;k<nrunUA;k++)
		paraUA[k] = para[nrunA + k][j];
		sort1(nrunA,paraA);
		sort1(nrunUA,paraUA);
		for(k=0;k<nrunA;k++)
		{
		fprintf(stream,"%d%10.4f%10.4f%10.4f%10.4f\n",k + 1,paraA[k],(k + 1.0)/nrunUA,paraUA[k],(k + 1.0)/nrunUA);
		}
		if(nrunUA > nrunA)
		{
		fprintf(stream,"%d%10.4f%10.4f%10.4f%10.4f\n",nrunUA,paraA[nrunA - 1],nrunA*1.0/nrunUA,paraUA[nrunUA - 1],1.0);
		}

		fprintf(stream,"End:Parameter[%d]---\n",j);
		}//end j

		delete[] VOBFr,paraA,paraUA;
		de_allocate(para,nrun);
		*/
		fclose(stream);
		//-----------------------------------------------------
		/* //show MPSA resluts
		if(frmMPSAmap) delete frmMPSAmap;
		frmMPSAmap = new TfrmMPSAmap(Application);
		frmMPSAmap->Caption = "Sensitivity Figures [Model Runs = " + String(nrun) + "]";

		// frmMPSAmap->PageControl1->ActivePageIndex = 0; //TabSheet1

		frmMPSAmap->ImportMPSAfile(asFile.c_str());
		frmMPSAmap->ShowModal(); //show MPSA resluts
		*/
		//-----------------------------------------------------
	}//END: Run MPSA

	//444444444444444444444444444444444444444444444444444444444444444444444444444444
	else if(Func == 4)
	{//BEGIN: Run PUSA: Parameter Uncertainty by Significance Analysis
         //OR DLUE: Distinction Level Uncertainty Estimation
		nrun = MaxGeneration; //运行模型次数
		asFile = strDirSm + "pusa.out";
		if((stream=fopen(asFile.c_str(),"w+"))==NULL)//E:\\DTVGM_CB
		{
			ShowMessage("Cannot open input file.");
			return;
		}

		int ncols,nland,nobj; //number of column, number of land use
		double dNR, dRS, dRSS, dETa;//dNR: Normal distribution Random number

		double miu[] = {3.0,1.39,1.63}; //BagrovN均值，林地、草地、耕地
		double sigma[] ={0.1,0.046,0.054}; //BagrovN方差，林地、草地、耕地

		nland = 3; // (Forest,Grass,Dry-land)
		nobj = 5; // (BagrovN,RS,RSS,R,ETa)
		ncols = nobj*nland; //(BagrovN,RS,RSS,R,ETa)*(Forest,Grass,Dry-land)
		double **para;
		para = new double*[nrun]; // STEP 1: SET UP THE ROWS.
		for (i = 0; i < nrun; i++)
			para[i] = new double[ncols]; // STEP 2: SET UP THE COLUMNS

		AnsiString paraName[] ={"BvNForest","BvNGrass","BvNDryland"};

		fprintf(stream,"Basin: %s\n",strStation);
		fprintf(stream,"Model_Runs: %d\n",nrun);
		fprintf(stream,"ParaNumbers: %d\n",nland);
		fprintf(stream,"ColNumbers: %d\n",ncols + 1);

		fprintf(stream,"\nLandUse\tMiu\tSigma\n");
		for(j=0;j<nland;j++)
			fprintf(stream,"%s%10.4f%10.4f\n",paraName[j],miu[j],sigma[j]);

		fprintf(stream,"\nMonteCarlo-Simulation-Results\n");

		fprintf(stream,"i\t\tBvNF\t\tRSF\t\tRSSF\t\tETaF\t\t"); //Forest land
		fprintf(stream,"BvNG\t\t\RSG\t\tRSSG\t\tETaG\t\t"); //Grassland
		fprintf(stream,"BvND\t\tRSD\t\tRSSD\t\tETaD\n"); //Dry-land

		for(j=0;j<3;j++)//Forest, Grass, Dry-land
		{randomize(); //Initializes the random number generator with a random value.
		for(i=0;i<nrun;i++)
		{
			dNR = NormalRandom(miu[j], sigma[j]);
			for(int ik=0;ik<ngrid;ik++)
				gridBagrovN[ik] = dNR;

			DTVGM_Month(nBasinGrid,ngrid,dateBegin,dateEnd,
				strDirRain,strPrefix_rainfile,
				nBagrovN,ArrayofBagrovN,gridBagrovN,
				gridSubID,gridArea,gridRCC,
				Array_ETp,RunoffObs,
				Parameter,Parameter_optimize,ProgressBar1,
				Mean_gridrain,Mean_gridRunoff,Mean_gridSurfaceRunoff,Mean_gridSubsurfaceRunoff,
				Mean_wateruse,Mean_gridETa,Mean_gridsm,RunoffCal,WB,NSEC);

			para[i][nobj*j + 0] = dNR;
			para[i][nobj*j + 1] = Sum(Mean_gridSurfaceRunoff,nMonths - 1);
			para[i][nobj*j + 2] = Sum(Mean_gridSubsurfaceRunoff,nMonths - 1);
			para[i][nobj*j + 3] = Sum(Mean_gridRunoff,nMonths - 1);
			para[i][nobj*j + 4] = Sum(Mean_gridETa,nMonths - 1);

		}//end i
		}//end j

		for(i=0;i<nrun;i++)
		{
			fprintf(stream,"%d",i + 1);
			for(j=0;j<ncols;j++)
				fprintf(stream,"%10.2f",para[i][j]);
			fprintf(stream,"\n");
		}

		de_allocate(para,ncols);
		fclose(stream);
                asFinish = "Distinction Level Uncertainty Estimation Finished";
	}//END: Run PUSA: Parameter Uncertainty by Significance Analysis

	//555555555555555555555555555555555555555555555555555555555555555555555555555555
	else if(Func == 5)
	{//BEGIN: Run HMCM: Hessian Matrix, Covariance Matrix of Parameters
		asFile = strDirSm + "HMCM.out";
		if((stream=fopen(asFile.c_str(),"w+"))==NULL)//E:\\DTVGM_CB
		{
			ShowMessage("Cannot open input file.");
			return;
		}

		//double minVOF;
		const double hratio = 0.05;
		//const double minVOF = 111.27;//146.0; //ObjectFunction += (sqrt(RunoffObs[i])-sqrt(RunoffCal[i]))*(sqrt(RunoffObs[i])-sqrt(RunoffCal[i]));
		double SD,HWCI_SD,SDpara[nopt];
		double ParaInput[nopt];
		double Xh, Yh, ZXY, ZXhY, ZXYh, ZXhYh;
		double **HesM;
		double **CovM;
		HesM=(double **)malloc(nopt*sizeof(double *));
		CovM=(double **)malloc(nopt*sizeof(double *));
		for(i=0;i<nopt;i++)
		{
			HesM[i]=(double *)malloc(nopt*sizeof(double));
			CovM[i]=(double *)malloc(nopt*sizeof(double));
		}

		ZXY = DTVGM_Month(nBasinGrid,ngrid,dateBegin,dateEnd,
			strDirRain,strPrefix_rainfile,
			nBagrovN,ArrayofBagrovN,gridBagrovN,
			gridSubID,gridArea,gridRCC,
			Array_ETp,RunoffObs,
			Parameter,Parameter_optimize,ProgressBar1,
			Mean_gridrain,Mean_gridRunoff,Mean_gridSurfaceRunoff,Mean_gridSubsurfaceRunoff,
			Mean_wateruse,Mean_gridETa,Mean_gridsm,RunoffCal,WB,NSEC);

		//ZXY = 108.58;
		//ZXY = 140;
		VOBF = ZXY;
		double noptINV = nopt;//nopt - 1; //the last parameter AW0 is not sensitive

		fprintf(stream,"ParaName\tParaIncr\t\t ZX\t\t ZXh\t\t ZXhh\t\t HesM\n");
		for(i=0; i<nopt; i++)
		{
			Xh = hratio*Parameter_optimize[i];
			for(k=0; k<nopt; k++)
				ParaInput[k] = Parameter_optimize[k];

			ParaInput[i] = Parameter_optimize[i] + Xh;
			ZXhY = DTVGM_Month(nBasinGrid,ngrid,dateBegin,dateEnd,
				strDirRain,strPrefix_rainfile,
				nBagrovN,ArrayofBagrovN,gridBagrovN,
				gridSubID,gridArea,gridRCC,
				Array_ETp,RunoffObs,
				Parameter,ParaInput,ProgressBar1,
				Mean_gridrain,Mean_gridRunoff,Mean_gridSurfaceRunoff,Mean_gridSubsurfaceRunoff,
				Mean_wateruse,Mean_gridETa,Mean_gridsm,RunoffCal,WB,NSEC);

			ParaInput[i] = Parameter_optimize[i] + 2*Xh;
			ZXhYh = DTVGM_Month(nBasinGrid,ngrid,dateBegin,dateEnd,
				strDirRain,strPrefix_rainfile,
				nBagrovN,ArrayofBagrovN,gridBagrovN,
				gridSubID,gridArea,gridRCC,
				Array_ETp,RunoffObs,
				Parameter,ParaInput,ProgressBar1,
				Mean_gridrain,Mean_gridRunoff,Mean_gridSurfaceRunoff,Mean_gridSubsurfaceRunoff,
				Mean_wateruse,Mean_gridETa,Mean_gridsm,RunoffCal,WB,NSEC);

			HesM[i][i] = HessianXY(Xh,ZXY,ZXhY,ZXhYh);
			fprintf(stream,"%5d%10.6f%10.2f%10.2f%10.2f%15.2f\n",i + 1,Xh,ZXY,ZXhY,ZXhYh,HesM[i][i]);
		} //end i

		fprintf(stream,"\nParaX\tParaY\tParaXIncr\tParaYIncr\t\t ZXY\t\t ZXhY\t\t ZXYh\t\t ZXhYh\t\t HesM\n");
		for(i=0; i<nopt; i++)
			for(j=i + 1; j<nopt; j++)
			{
				Xh = hratio*Parameter_optimize[i];
				Yh = hratio*Parameter_optimize[j];

				for(k=0; k<nopt; k++)
					ParaInput[k] = Parameter_optimize[k];

				ParaInput[i] = Parameter_optimize[i] + Xh;
				ParaInput[j] = Parameter_optimize[j];
				ZXhY = DTVGM_Month(nBasinGrid,ngrid,dateBegin,dateEnd,
					strDirRain,strPrefix_rainfile,
					nBagrovN,ArrayofBagrovN,gridBagrovN,
					gridSubID,gridArea,gridRCC,
					Array_ETp,RunoffObs,
					Parameter,ParaInput,ProgressBar1,
					Mean_gridrain,Mean_gridRunoff,Mean_gridSurfaceRunoff,Mean_gridSubsurfaceRunoff,
					Mean_wateruse,Mean_gridETa,Mean_gridsm,RunoffCal,WB,NSEC);

				ParaInput[i] = Parameter_optimize[i];
				ParaInput[j] = Parameter_optimize[j] + Yh;
				ZXYh = DTVGM_Month(nBasinGrid,ngrid,dateBegin,dateEnd,
					strDirRain,strPrefix_rainfile,
					nBagrovN,ArrayofBagrovN,gridBagrovN,
					gridSubID,gridArea,gridRCC,
					Array_ETp,RunoffObs,
					Parameter,ParaInput,ProgressBar1,
					Mean_gridrain,Mean_gridRunoff,Mean_gridSurfaceRunoff,Mean_gridSubsurfaceRunoff,
					Mean_wateruse,Mean_gridETa,Mean_gridsm,RunoffCal,WB,NSEC);

				ParaInput[i] = Parameter_optimize[i] + Xh;
				ParaInput[j] = Parameter_optimize[j] + Yh;
				ZXhYh = DTVGM_Month(nBasinGrid,ngrid,dateBegin,dateEnd,
					strDirRain,strPrefix_rainfile,
					nBagrovN,ArrayofBagrovN,gridBagrovN,
					gridSubID,gridArea,gridRCC,
					Array_ETp,RunoffObs,
					Parameter,ParaInput,ProgressBar1,
					Mean_gridrain,Mean_gridRunoff,Mean_gridSurfaceRunoff,Mean_gridSubsurfaceRunoff,
					Mean_wateruse,Mean_gridETa,Mean_gridsm,RunoffCal,WB,NSEC);

				HesM[i][j] = HessianXY(Xh,Yh,ZXY,ZXhY,ZXYh,ZXhYh);
				HesM[j][i] = HesM[i][j];

				fprintf(stream,"%5d%5d%10.6f%10.6f%10.2f%10.2f%10.2f%10.2f%15.2f\n",i + 1,j + 1,Xh,Yh,ZXY,ZXhY,ZXYh,ZXhYh,HesM[i][j]);
			} //end i,j

			fprintf(stream,"\n");
			for(i=0;i<nopt;i++)
				for(j=0;j<nopt;j++)
					CovM[i][j] = HesM[i][j];


			MatrixINV(CovM,noptINV);

			SD = sqrt(VOBF/(nMonths - nopt));
			HWCI_SD = 1.38*SD/sqrt(nMonths - nopt);

			for(i=0;i<noptINV;i++)
				for(j=0;j<noptINV;j++)
					CovM[i][j] = 2*SD*SD*CovM[i][j];

			for(i=0;i<noptINV;i++)
				SDpara[i] = sqrt(CovM[i][i]); //standard deviation of Parameters

			fprintf(stream,"minVOF\tN(Number of terms)\tK(Number of Parameters)\t:\n");
			fprintf(stream,"%10.2f%10d%10d\n",VOBF,nMonths,nopt);

			fprintf(stream,"\nSD(Model_Strandard_Deviation)\tHWCI_SD(half width of a 95% Confidence Interval):\n");
			fprintf(stream,"%10.4f%10.4f\n",SD,HWCI_SD);

			fprintf(stream,"\nParaName\tParaValue\tSDpara\tHWCIpara\tLowerBound\tHigherBound\n");
			for(i=0;i<noptINV;i++)
				fprintf(stream,"%7d%10.4f%10.4f%10.4f%10.4f%10.4f\n",i+1,Parameter_optimize[i],SDpara[i],1.96*SDpara[i],Parameter_optimize[i] - 1.96*SDpara[i],Parameter_optimize[i] + 1.96*SDpara[i]);

			fprintf(stream,"\nHessian Matrix:\n");
			for(i=0;i<nopt;i++)
			{
				//fprintf(stream,"%d",i + 1);
				for(j=0;j<nopt;j++)
					fprintf(stream,"%10.2f",HesM[i][j]);
				fprintf(stream,"\n");
			}

			fprintf(stream,"\nCovariance Matrix:\n");
			for(i=0;i<noptINV;i++)
			{
				//fprintf(stream,"%d",i + 1);
				for(j=0;j<noptINV;j++)
					fprintf(stream,"%15.6f",CovM[i][j]);
				fprintf(stream,"\n");
			}

			// fprintf(stream,"\n");
			for(i=0; i<noptINV; i++)
				for(j=i; j<noptINV; j++)
				{
					CovM[i][j] = CovM[i][j]/(SDpara[i]*SDpara[j]);
					CovM[j][i] = CovM[i][j];
				}

				fprintf(stream,"\nCorrelation Matrix:\n");
				for(i=0;i<noptINV;i++)
				{

					for(j=0;j<noptINV;j++)
						fprintf(stream,"%15.6f",CovM[i][j]);
					fprintf(stream,"\n");
				}

				de_allocate(HesM,nopt);
				de_allocate(CovM,nopt);
				fclose(stream);
                        asFinish = "Hessian and Covariance Matrix Calculation Finished";
	}//END: Run HMCM: Hessian Matrix, Covariance Matrix of Parameters

	//666666666666666666666666666666666666666666666666666666666666666666666666666666
	else if(Func == 6)
	{//BEGIN: Run MCMC: Metropolis Monte Carlo Markov Chain

                asFile = strDirSm + "MCMC.rec";  //MCMC record
                if((frec=fopen(asFile.c_str(),"w+"))==NULL)//E:\\DTVGM_CB
		{
			ShowMessage("Cannot open input file:" + asFile);
			return;
		}

		asFile = strDirSm + "MCMC.rlt";  //MCMC Output
		if((stream=fopen(asFile.c_str(),"w+"))==NULL)//E:\\DTVGM_CB
		{
			ShowMessage("Cannot open input file:" + asFile);
			return;
		}
		fprintf(frec,"BasinSelect: %s\n",gstrBasinSelect);

		int noptM = nopt + 1; ////g1,g2,Kr,KAW,AW0,Var
		int noptMC = 0; //g1,Kr,KAW,Var
		int *noptI = new int[noptM];
		double *theta0 = new double[noptM];
		double *theta = new double[noptM]; //g1,g2,Kr,KAW,AW0
		double *thetamin = new double[noptM];
		double *thetamax = new double[noptM];
		double Xmin,Xmax;

		long j;
		int i,isel0,isel;

		long *nAcc = new long[noptM]; //number of Acceptance
		long *nSel = new long[noptM]; //number of Selected


		for(i=0;i<nopt;i++)
		{
			theta0[i] = Parameter_optimize[i];
			thetamin[i] = Variable_min[i];
			thetamax[i] = Variable_max[i];
		}

		theta0[noptM - 1] = 0.3; //Var0 = 0.3;
		thetamin[noptM - 1] = 0.05;
		thetamax[noptM - 1] = 2.00;

		for(i=0;i<noptM;i++) theta[i] = theta0[i];

		String strParaEn,strValue;
		int iProDis; //ProposalDistribution selection
		long Nsamples;
		double Sigma,Tao;// = 0.05; //variation amplitude: [theta0[isel] - r0,theta0[isel] + r0]
		// Nsamples = MaxGeneration;// = 100000;

		if(DM->ADOTable0->Filtered) DM->ADOTable0->Filtered = false;
		DM->ADOTable0->Active = false;
		DM->ADOTable0->TableName = "MCMCini";
		DM->ADOTable0->Active = true;
		DM->ADOTable0->First();

		for(int i=0;i<DM->ADOTable0->RecordCount;i++)
		{//DM->ADOTable0->Edit();
			strParaEn = DM->ADOTable0->FieldValues["ITEM"];
			strValue = DM->ADOTable0->FieldValues["Value"];
			strParaEn = Trim(strParaEn);
			strValue = Trim(strValue);
			fprintf(frec,"%s\t\t%s\n",strParaEn,strValue);
			if(strParaEn == "g1")
			{if(strValue =="True")
			{noptMC +=1;noptI[noptMC - 1] = 0;}}
			else if (strParaEn == "g2")
			{if(strValue =="True")
			{noptMC +=1;noptI[noptMC - 1] = 1;}}
			else if (strParaEn == "Kr")
			{if(strValue =="True")
			{noptMC +=1;noptI[noptMC - 1] = 2;}}
			else if (strParaEn == "KAW")
			{if(strValue =="True")
			{noptMC +=1;noptI[noptMC - 1] = 3;}}
			else if (strParaEn == "AW0")
			{if(strValue =="True")
			{noptMC +=1;noptI[noptMC - 1] = 4;}}
			else if (strParaEn == "ProposalDistribution")
				iProDis = strValue.ToInt();
			else if (strParaEn == "Samples")
				Nsamples = strValue.ToDouble();
			else if(strParaEn == "Sigma")
				Sigma = strValue.ToDouble();
			DM->ADOTable0->Next();
		}

		noptMC += 1; //Var
		noptI[noptMC - 1] = noptM - 1; //Var

		Randomize();

		for(i=0;i<noptM;i++){nAcc[i] = 0; nSel[i] = 0;}

		double ProbAcc0,ProbAcc,RateAcc; //VarE,
		double *Rcal0 = new double[nMonths];

		fprintf(frec,"\nIsample\t\tIsel\t\tTheta[1:noptMC]\t\tRunoffCal[1:nMonths]\n");

//		ShowMessage("MCMC BEGIN: M_PI = "+FloatToStr(M_PI));


		// modelabc(nn,theta0,Pobs,Robs,GWobs,Rcal0,GWcal,NSEC_R,NSEC_GW);
		DTVGM_Month_MCMC(nBasinGrid,ngrid,dateBegin,dateEnd,
			strDirRain,strPrefix_rainfile,
			nBagrovN,ArrayofBagrovN,gridBagrovN,
			gridSubID,gridArea,gridRCC,
			Array_ETp,RunoffObs,
			Parameter,theta0,ProgressBar1,
			Mean_gridrain,Mean_gridRunoff,Mean_gridSurfaceRunoff,Mean_gridSubsurfaceRunoff,
			Mean_wateruse,Mean_gridETa,Mean_gridsm,Rcal0,WB,NSEC);


		for(j=0;j<Nsamples;j++)
		{
			for(i=0;i<noptMC;i++) theta[noptI[i]] = theta0[noptI[i]];
			isel0 = random(noptMC); //只对theta[0],...,theta[noptMC - 1]操作
			isel = noptI[isel0];
			nSel[isel] += 1;
			switch (iProDis)
			{
			case 0: //symmetric uniform
				{
					Xmin = max(0.001,theta0[isel] - Sigma);
					Xmax = max(2.0*Sigma,theta0[isel] + Sigma);
					theta[isel] = Xmin + (Xmax - Xmin)*random(100)/100.0;
					break;
				}
			case 1: //asymmetric uniform
				{//need to be coded
                                        Xmin = max(0.001,theta0[isel]/(1.0+Sigma));
					Xmax = max(2.0*Sigma,theta0[isel]*(1.0+Sigma));
			     //		Xmin = max(thetamin[isel],theta0[isel]/(1.0+Sigma));
			     //		Xmax = min(thetamax[isel],theta0[isel]*(1.0+Sigma));
					theta[isel] = Xmin + (Xmax - Xmin)*random(100)/100.0;
					break;
				}
			case 2: //symmetric normal
				{//Standard Deviation = 0.5*(Max - Min)/Sigma
					theta[isel] = RandG(theta0[isel],0.5*(thetamax[isel] - thetamin[isel])/Sigma);
                                        theta[isel] = max(0.001,theta[isel]);
                                        //theta[isel] = min(thetamax[isel],theta[isel]);
                                        //if(theta[isel]>thetamax[isel]||theta[isel]<thetamin[isel])
                                        //        theta[isel] = thetamin[isel] + (thetamax[isel] - thetamin[isel])*random(100)/100.0;
					break;
				}
			case 3: //asymmetric normal
				{//need to be coded
					Tao = (thetamax[isel] - thetamin[isel])/(0.5*(thetamax[isel] + thetamin[isel]))/(2*Sigma);
					theta[isel] = RandG(theta0[isel],Tao*theta0[isel]);
					theta[isel] = max(thetamin[isel],theta[isel]);
                                        theta[isel] = min(thetamax[isel],theta[isel]);
					break;
				}
			}//end switch

			DTVGM_Month_MCMC(nBasinGrid,ngrid,dateBegin,dateEnd,
				strDirRain,strPrefix_rainfile,
				nBagrovN,ArrayofBagrovN,gridBagrovN,
				gridSubID,gridArea,gridRCC,
				Array_ETp,RunoffObs,
				Parameter,theta,ProgressBar1,
				Mean_gridrain,Mean_gridRunoff,Mean_gridSurfaceRunoff,Mean_gridSubsurfaceRunoff,
				Mean_wateruse,Mean_gridETa,Mean_gridsm,RunoffCal,WB,NSEC);

                        fprintf(stream,"%d\t%d\t%d\t",j,isel0,isel);

			ProbAcc = AccProb(iProDis,isel,noptM,theta0,theta,Tao,nMonths,RunoffObs,Rcal0,RunoffCal);
                        ProbAcc0 = random(1000)/1000.0;
                        fprintf(stream,"%6.2f\t%6.2f\t",ProbAcc,ProbAcc0);
			if(ProbAcc > ProbAcc0)//Accept
			{
				nAcc[isel] += 1;
				for(i=0;i<noptMC;i++) theta0[noptI[i]] = theta[noptI[i]];
				for(i=0;i<nMonths;i++) Rcal0[i] = RunoffCal[i];
                                fprintf(stream,"Y\t");
			}
                        else fprintf(stream,"N\t");

			// llh0 = llh;


                        
			for(i=0;i<noptMC;i++) fprintf(stream,"%10.4f\t",theta0[noptI[i]]);
			fprintf(stream,"\t");
			for(i=0;i<nMonths;i++) fprintf(stream,"%10.2f\t",Rcal0[i]);
			fprintf(stream,"\n");
		} //end j

		for(i=0;i<noptM;i++)
		{
			RateAcc = nSel[i]>0?nAcc[i]*1.0/nSel[i]*100:0;
			fprintf(frec,"\n%10.4f,%d, %d,Acceptance Rate[%d] = %5.2f%\t\t",theta0[i],nSel[i],nAcc[i],i,RateAcc);
		}

                fclose(frec);
		fclose(stream);

		delete[] theta0;
		delete[] theta;
		delete[] nAcc;
		delete[] Rcal0;
		delete[] thetamin;
		delete[] thetamax;
		delete[] nSel;
		delete[] noptI;

		asFinish = "Metropolis Monte Carlo Markov Chain Simulation Finished";

	} //END: Run MCMC: Metropolis Monte Carlo Markov Chain


	//------------------------------------------------------------------------------
	ADOTable0->First();
	for(i=0;i<nMonths;i++)
	{ ADOTable0->Edit();
	ADOTable0->FieldValues["qcal"] = RunoffCal[i];

	ADOTable0->FieldValues["MeanGridRain"] = Mean_gridrain[i];
	ADOTable0->FieldValues["Runoff"] = Mean_gridRunoff[i];
	ADOTable0->FieldValues["SurfaceRunoff"] = Mean_gridSurfaceRunoff[i];
	ADOTable0->FieldValues["SubSurfaceRunoff"] = Mean_gridSubsurfaceRunoff[i];
	ADOTable0->FieldValues["ETa"] = Mean_gridETa[i];
	ADOTable0->FieldValues["AWmean"] = Mean_gridsm[i];
	ADOTable0->FieldValues["Wateruse"] = Mean_wateruse[i];

	ADOTable0->FieldValues["WaterBalance"] = 0;
	if(i>0) ADOTable0->FieldValues["WaterBalance"] =
		Mean_gridsm[i - 1] + Mean_gridrain[i]- Mean_gridRunoff[i]- Mean_gridETa[i]- Mean_wateruse[i] - Mean_gridsm[i];


	ADOTable0->Next();
	}//i

	ProgressBar1->Visible = false;

	DateTimeToString(strtemp,"yyyymmddhhmmss",Now());
	asFile = strDirResult + strPrefix_resultfile + strStation + strtemp + ".txt";
	//strtemp = asFile;
	txtFileName = asFile.c_str();
	ExportFile(ADOTable0,txtFileName);
	//asFile = strPrefix_resultfile + strtemp + ".txt"; //DTVGMlog里面存储文件名，不要路径

	delete[] gridBagrovN;
	delete[] Mean_gridSurfaceRunoff;
	delete[] Mean_gridSubsurfaceRunoff;
	delete[] Mean_gridRunoff;
	delete[] Mean_gridETa;
	delete[] Mean_gridsm;

	delete[] RunoffObs;
	delete[] RunoffCal;
	delete[] RunoffCal0;
	delete[] Array_ETp;
	delete[] ArrayofBagrovN;


	RunEnd = Now(); time_End = time(NULL);
	SpendTime = difftime(time_End,time_Start);
	strtemp = IntToStr(SpendTime/3600) + ":" +
		IntToStr((SpendTime - SpendTime/3600*3600)/60) + ":" +
		IntToStr(SpendTime - SpendTime/3600*3600 - (SpendTime - SpendTime/3600*3600)/60*60);
	Edit1->Text = asFinish + "; 水量平衡指数RSO = " + FloatToStrF(WB,ffFixed,4,2)+
		"; Nash效率系数NSEC = " + FloatToStrF(NSEC,ffFixed,4,2)+
		"; 目标函数VOBF = " + FloatToStrF(VOBF,ffFixed,8,2); //FloatToStrF(FPoint[1].x,ffFixed,4,1)
	DTVGMlog(DM->ADOTable3,asFile,WB,NSEC,
		AW0,AWC,g1,g2,Kr,PDc,PDc_basic,KAW,KET,dateBegin,dateEnd,RunStart,RunEnd,strtemp);
//	ShowMessage("Simulation Completed, VOBF = " + (String)VOBF);

}


//********************************************************
//********************************************************
//********************************************************
//*********************************************************
void __fastcall TMDIChild::Optimize_GA()
{
	//Variable_min[] = {0,0,0,0,0};
	//Variable_max[] = {8,8,8,8,8};
	int i,j;
	long ltemp;
	if((fpga0=fopen("E:\\DTVGM_CB\\TEXTFILE\\ga_output.txt","w+"))==NULL)
	{
		fprintf(stderr,"Cannot open input file.\n");
		return;
	}

	generation=0;
	GenerateInitialPopulation();
	//begin: EvaluatePopulation();

	//CalculateObjectValue()
	for (i=0;i<POPSIZE;i++)
	{
		for(j=0;j<NUMV;j++)
		{
			ltemp = DecodeChromosome(population[i].chrom,j*LENGTH,LENGTH);
			Vx[j] = VariableValue(ltemp,Variable_min[j],Variable_max[j]);
		} //j

		population[i].value=pow(Vx[0]-1.0,2) + pow(Vx[1]-2.0,2) + pow(Vx[2]-3.0,2);// + pow(Vx[3]-4.0,2) + pow(Vx[4]-5.0,2);

	} //end for CalculateObjectValue()
	CalculateFitnessValue( ); /* calculate fitness value */
	FindBestAndWorstIndividual( );/* find the best and worst individual */
	//end: EvaluatePopulation();


	while (generation<MaxGeneration)
	{
		generation++;
		GenerateInitialPopulation();

		//begin: EvaluatePopulation();
		//CalculateObjectValue()
		for (i=0;i<POPSIZE;i++)
		{
			for(j=0;j<NUMV;j++)
			{
				ltemp = DecodeChromosome(population[i].chrom,j*LENGTH,LENGTH);
				Vx[j] = VariableValue(ltemp,Variable_min[j],Variable_max[j]);
			} //j

			population[i].value=pow(Vx[0]-1.0,2) + pow(Vx[1]-2.0,2) + pow(Vx[2]-3.0,2);// + pow(Vx[3]-4.0,2) + pow(Vx[4]-5.0,2);

		} //end for CalculateObjectValue()
		CalculateFitnessValue(); /* calculate fitness value */
		FindBestAndWorstIndividual();/* find the best and worst individual */
		//end: EvaluatePopulation();

		PerformEvolution();
		OutputTextReport ();
	} //end while
	fclose(fpga0);
	//END: Run Genetic Algorithm
	ShowMessage("Excellent");
}



//------------------------------------------------------
void __fastcall TMDIChild::Run_LandFit_Simple()
{ //a Simple Method
	AnsiString asFile,strtemp; // 如果定义AnsiString，将出现异常
	char *txtFileName;
	FILE *stream;
	int iFileHandle;
	/*
	String strAlias,strTable,strField,strFilter;
	strAlias = DM->ADOConnection1->DefaultDatabase;
	strTable = "LandFit";
	strField = "GridID";
	TDate dateBegin,dateEnd;
	*/
	TDateTime RunStart,RunEnd;
	time_t time_Start,time_End;
	int SpendTime; //seconds
	const int ngrid = 32779;
	MaxGeneration = StrToInt(InputBox("Input MaxGeneration", "Prompt", "2"));

	const int nLand = 5;
	int LandID[nLand] = {1,2,3,5,6}; //Crop,Forest,Crop,Urban,noUse
	int Landnum[nLand],Landnum_cal[nLand]; //每种类型的栅格数目

	//long ltemp;
	double VOBF;

	int i,j,k;

	double ITER[nLand] ={0,0,0,0,0};

	ProgressBar1->Visible = true;
	//-------------------------------------------------

	int GridID;
	float coordx,coordy,FitForest0,FitGrass0,FitCrop0,FitUrban0,FitNoUse0,Land10,Land20;
	double *FitForest = new double[ngrid];
	double *FitGrass = new double[ngrid];
	double *FitCrop = new double[ngrid];
	double *FitUrban = new double[ngrid];
	double *FitNoUse = new double[ngrid];
	int *Land1 = new int[ngrid];
	int *Land2 = new int[ngrid];
	int *Land2_cal = new int[ngrid];

	for(j=0;j<nLand;j++)
	{
		Landnum[j] = 0;
	}

	/*
	strFilter = strField + ">0";
	SetFilter(true,strAlias,strTable,strField,strFilter);
	ADOTable0->First();
	for(i=0;i<ngrid;i++)
	{
	FitForest[i] = ADOTable0->FieldValues["FitForest"];
	FitGrass[i] = ADOTable0->FieldValues["FitGrass"];
	FitCrop[i] = ADOTable0->FieldValues["FitCrop"];
	FitUrban[i] = ADOTable0->FieldValues["FitUrban"];
	FitNoUse[i] = ADOTable0->FieldValues["FitNoUse"];
	Land1[i] = ADOTable0->FieldValues["Land1987"];
	Land2[i] = ADOTable0->FieldValues["Land2000"];
	for(j=0;j<nLand;j++)
	{
	if(Land2[i]==LandID[j]) Landnum[j] += 1;
	}
	ADOTable0->Next();
	ProgressBar1->Position = 100.*i/ngrid;
	} */

	asFile = "E:\\DTVGM_CB\\TEXTFILE\\table_landfit01.txt";//Open File: Gridrain 0<landfit<1

	if(FileExists(asFile))
	{
		if((stream=fopen(asFile.c_str(),"rt"))==NULL)
		{
			fprintf(stderr,"Cannot open input file.\n");
			return;
		}
	}
	else
	{
		ShowMessage(AnsiString("Couldnot find the file:")+ asFile +".");
		return;
	}

	for(i=0;i<ngrid;i++)// !feof(stream)
	{ fscanf(stream,"%d %f %f %f %f %f %f %f %f %f",&GridID,&coordx,&coordy,
	&FitForest0,&FitGrass0,&FitCrop0,&FitUrban0,&FitNoUse0,&Land10,&Land20); // fscanf(stream,"\n");
	FitForest[i] = FitForest0;
	FitGrass[i] = FitGrass0;
	FitCrop[i] = FitCrop0;
	FitUrban[i] = FitUrban0;
	FitNoUse[i] = FitNoUse0;
	Land1[i] = Land10;
	Land2[i] = Land20;
	for(j=0;j<nLand;j++)
	{
		if(Land2[i]==LandID[j]) Landnum[j] += 1;
	}
	ProgressBar1->Position = 100.*i/ngrid;
	}
	fclose(stream);
	//-------------------------------------------------
	RunStart = Now(); time_Start = time(NULL);



	{//BEGIN: Run Simple Algorithm

		VOBF = LandFit_Simple(MaxGeneration,ngrid,nLand,ITER,LandID,Landnum,Landnum_cal,
			FitForest,FitGrass,FitCrop,FitUrban,FitNoUse,
			Land1,Land2,Land2_cal);

		// fclose(fpga0);


	} //END: Run Simple Algorithm



	ProgressBar1->Visible = false;

	DateTimeToString(strtemp,"yyyymmddhhmmss",Now());
	//最优结果存为文本文件
	asFile = "E:\\DTVGM_CB\\TEXTFILE\\LUCC" + strtemp + ".txt";
	txtFileName = asFile.c_str();

	if(!FileExists(txtFileName))
	{iFileHandle = FileCreate(txtFileName);FileClose(iFileHandle);}
	if((stream = fopen(txtFileName, "w+"))==NULL)
	{
		ShowMessage("File Open Failed!!!");
		return;
	}

	for(i=0;i<nLand;i++)
	{
		fprintf(stream,"%4.2f\t",ITER[i]);
	}
	fprintf(stream, "\n");

	for(j=0;j<ngrid;j++)
	{
		fprintf(stream,"%d\t %d\t %d\t %d",j,Land1[j],Land2[j],Land2_cal[j]);
		fprintf(stream, "\n");
	}
	//Table1->Post();
	fclose(stream);

	delete[] FitForest;
	delete[] FitGrass;
	delete[] FitCrop;
	delete[] FitUrban;
	delete[] FitNoUse;
	delete[] Land1;
	delete[] Land2;
	delete[] Land2_cal;


	RunEnd = Now(); time_End = time(NULL);
	SpendTime = difftime(time_End,time_Start);
	strtemp = IntToStr(SpendTime/3600) + ":" +
		IntToStr((SpendTime - SpendTime/3600*3600)/60) + ":" +
		IntToStr(SpendTime - SpendTime/3600*3600 - (SpendTime - SpendTime/3600*3600)/60*60);
	Edit1->Text = "目标函数VOBF = " + FloatToStrF(VOBF,ffFixed,4,2) + "; 耗时：" + strtemp; //FloatToStrF(FPoint[1].x,ffFixed,4,1)

	ShowMessage("Simulation Completed, VOBF = " + (String)VOBF);

}
//---------------------------------------------------------------------------
extern double __fastcall LandFit_Simple(int MaxStep,int ngrid,int nLand,double ITER[],int LandID[],int *Landnum,int *Landnum_cal,
										double FitForest[],double FitGrass[],double FitCrop[],double FitUrban[],double *FitNoUse,
										int Land1[],int Land2[],int Land2_cal[])
{//compute Value of Objective Function
	double VOF,maxFitness;
	//int LandID[nLand] = {1,2,3,5,6}; //nLand = 5; Crop,Forest,Crop,Urban,noUse
	//int Landnum[nLand] ,上面每种LUCC类型的栅格数目，代表面积
	double *Fitness = new double[nLand];
	double *gridFit = new double[ngrid];
	//int *Landnum_cal = new int[nLand]; //计算的每种LUCC个数
	//double *gridFit2 = new double[ngrid];
	//int *gridid2 = new int[ngrid];
	FILE *fpfile;
	int i,j,k,Step;
	int bestStep;
	double tempVOF;
	VOF = 1.0;
	const double StepLen = 0.01;

	if((fpfile=fopen("E:\\DTVGM_CB\\TEXTFILE\\LUCC_output_Simple.txt","w+"))==NULL)
	{
		fprintf(stderr,"Cannot open input file.\n");
		return 100;
	}
	for(j=0;j<nLand;j++)
		fprintf(fpfile," LandNUM_2000[ %d ] = %d,",j,Landnum[j]);
	fprintf(fpfile,"\n");



	Step = 0;
	while (Step<MaxStep)
	{fprintf(fpfile," Step=%d",Step);
	fprintf(fpfile,"\n");
	for(i=0;i<ngrid;i++)
	{
		Fitness[0] = FitCrop[i] + ITER[0];
		Fitness[1] = FitForest[i] + ITER[1];
		Fitness[2] = FitGrass[i] + ITER[2];
		Fitness[3] = FitUrban[i] + ITER[3];
		Fitness[4] = FitNoUse[i] + ITER[4];
		maxFitness = MaxValue(Fitness,nLand - 1);
		gridFit[i] = maxFitness;
		for(j=0;j<nLand;j++)
		{
			if(maxFitness==Fitness[j])
				Land2_cal[i] = LandID[j];
		}

	}//end for

	k=0;
	for(i=0;i<ngrid;i++)
	{
		if(Land2_cal[i] == Land2[i]) k++;
	}

	tempVOF = 1 - k*1.0/ngrid; //错误率 ，优化最小值
	if(tempVOF<VOF)
	{
		VOF = tempVOF;
		bestStep = Step;
	}

	for(j=0;j<nLand;j++)
	{Landnum_cal[j]=0;
	for(i=0;i<ngrid;i++)//找出每种LUcc的栅格
	{
		if(Land2_cal[i] == LandID[j])
		{
			Landnum_cal[j]++;
		} //end if
	} //end for
	fprintf(fpfile," Diff_LandNUM[ %d ] = %7.2f%,",j,(Landnum_cal[j] - Landnum[j])*100.0/Landnum[j]);
	} //end for

	fprintf(fpfile,"\n");
	for(j=0;j<nLand;j++)
	{ fprintf(fpfile," ITER[ %d ] = %f,",j,ITER[j]);
	if(Landnum_cal[j]<Landnum[j])// && fabs(Landnum_cal[j] - Landnum[j])/Landnum[j] > 0.05
		ITER[j] += StepLen*fabs(Landnum_cal[j] - Landnum[j])/Landnum[j]; //该种Lucc数据不够，调节系数增加
	else if(Landnum_cal[j] > Landnum[j]) // && fabs(Landnum_cal[j] - Landnum[j])/Landnum[j] > 0.05
		ITER[j] -= StepLen*fabs(Landnum_cal[j] - Landnum[j])/Landnum[j]; //该种Lucc数据不够，调节系数减小
	}//end for
	fprintf(fpfile,"\n");
	Step++;
	}//end while

	fprintf(fpfile," bestVOF[ %d ] = %f,",bestStep,VOF);
	fclose(fpfile);


	delete[] Fitness;
	delete[] gridFit;
	//delete[] gridFit2;
	//delete[] gridid2;

	return VOF;
}

extern void __fastcall sortASC(int nArray,double *Array,int *Arrayi)
{//从小到大排序
	double dltemp;
	int itemp;
	for(int i=0;i<nArray;i++)
	{
		Arrayi[i] = i;
	}

	for(int i=0;i<nArray - 1;i++)
	{
		for(int j=i+1;j<nArray;j++)
		{
			if(Array[j]<Array[i])
			{
				itemp = Arrayi[i];
				dltemp = Array[i];

				Array[i] = Array[j];
				Arrayi[i] = Arrayi[j];

				Array[j] = dltemp;
				Arrayi[j] = itemp;
			}
		}
	}
} //end sortASC

void __fastcall TMDIChild::GA_LUCC()
{
	double Array[10] = {2,1,5,6,3,9,7,8,4,0};
	int Arrayi[10];
	sortASC(10,Array,Arrayi);
	ShowMessage("ok");
}

extern void __fastcall EvaluatePopulation_LUCC(int ngrid,int nLand,double ITER[],int LandID[],int *Landnum,
											   double FitForest[],double FitGrass[],double FitCrop[],double FitUrban[],
											   int Land1[],int Land2[],int Land2_cal[])
{
	int i,j;
	long temp;

	for (i=0;i<POPSIZE;i++)
	{
		for(j=0;j<NUMV;j++)
		{
			temp = DecodeChromosome(population[i].chrom,j*LENGTH,LENGTH);
			ITER[j] = VariableValue(temp,Variable_min[j],Variable_max[j]);
		} //j

		population[i].value=LandFit(ngrid,nLand,ITER,LandID,Landnum,
			FitForest,FitGrass,FitCrop,FitUrban,
			Land1,Land2,Land2_cal);

	} //i
	CalculateFitnessValue( ); /* calculate fitness value */
	FindBestAndWorstIndividual( ); /* find the best and worst individual */
}

//------------------------------------------------------
void __fastcall TMDIChild::Run_LandFit(int Func)
{
	AnsiString asFile,strtemp; // 如果定义AnsiString，将出现异常
	char *txtFileName;
	FILE *stream;
	int iFileHandle;

	String strAlias,strTable,strField,strFilter;
	strAlias = DM->ADOConnection1->DefaultDatabase;
	strTable = "LandFit";
	strField = "GridID";
	TDate dateBegin,dateEnd;
	TDateTime RunStart,RunEnd;
	time_t time_Start,time_End;
	int SpendTime; //seconds
	const int ngrid = 32779;
	MaxGeneration = StrToInt(InputBox("Input MaxGeneration", "Prompt", "2"));

	const int nLand = 5;
	int LandID[nLand] = {1,2,3,5,6}; //Crop,Forest,Crop,Urban,noUse
	int Landnum[nLand - 1]; //除了noUse类型之外，每种类型的栅格数目

	//long ltemp;
	double VOBF;

	int i,j,k;

	double ITER[nLand - 1] ={0.1,0.1,0.1,0.1};

	ProgressBar1->Visible = true;
	//-------------------------------------------------


	double *FitForest = new double[ngrid];
	double *FitGrass = new double[ngrid];
	double *FitCrop = new double[ngrid];
	double *FitUrban = new double[ngrid];
	int *Land1 = new int[ngrid];
	int *Land2 = new int[ngrid];
	int *Land2_cal = new int[ngrid];

	for(j=0;j<nLand - 1;j++)
	{
		Landnum[j] = 0;
	}
	strFilter = strField + ">0";
	SetFilter(true,strAlias,strTable,strField,strFilter);
	ADOTable0->First();
	for(i=0;i<ngrid;i++)
	{
		FitForest[i] = ADOTable0->FieldValues["FitForest"];
		FitGrass[i] = ADOTable0->FieldValues["FitGrass"];
		FitCrop[i] = ADOTable0->FieldValues["FitCrop"];
		FitUrban[i] = ADOTable0->FieldValues["FitUrban"];
		Land1[i] = ADOTable0->FieldValues["Land1987"];
		Land2[i] = ADOTable0->FieldValues["Land2000"];
		for(j=0;j<nLand - 1;j++)
		{
			if(Land2[i]==LandID[j]) Landnum[j] += 1;
		}
		ADOTable0->Next();
		ProgressBar1->Position = 100.*i/ngrid;
	}
	//-------------------------------------------------
	RunStart = Now(); time_Start = time(NULL);

	if(Func == 0)
	{//BEGIN:DTVGM Simulation

		VOBF = LandFit(ngrid,nLand,ITER,LandID,Landnum,
			FitForest,FitGrass,FitCrop,FitUrban,
			Land1,Land2,Land2_cal);

	}//END: DTVGM Simulation

	else if(Func == 1)
	{//BEGIN: Run Genetic Algorithm
		if((fpga0=fopen("E:\\DTVGM_CB\\TEXTFILE\\LUCC_output.txt","w+"))==NULL)
		{
			fprintf(stderr,"Cannot open input file.\n");
			return;
		}

		generation=0;
		GenerateInitialPopulation();
		EvaluatePopulation_LUCC(ngrid,nLand,ITER,LandID,Landnum,
			FitForest,FitGrass,FitCrop,FitUrban,
			Land1,Land2,Land2_cal); // calculate object value

		while (generation<MaxGeneration)
		{
			generation++;
			GenerateInitialPopulation();

			EvaluatePopulation_LUCC(ngrid,nLand,ITER,LandID,Landnum,
				FitForest,FitGrass,FitCrop,FitUrban,
				Land1,Land2,Land2_cal); // calculate object value

			PerformEvolution();
			OutputTextReport ();
			ProgressBar1->Position = 100.*generation/MaxGeneration;
		} //end while
		fclose(fpga0);

		VOBF = LandFit(ngrid,nLand,Vx,LandID,Landnum,
			FitForest,FitGrass,FitCrop,FitUrban,
			Land1,Land2,Land2_cal);
	} //END: Run Genetic Algorithm



	ProgressBar1->Visible = false;

	DateTimeToString(strtemp,"yyyymmddhhmmss",Now());
	//最优结果存为文本文件
	asFile = "D:\\DTVGM_CB\\TEXTFILE\\LUCC" + strtemp + ".txt";
	txtFileName = asFile.c_str();

	if(!FileExists(txtFileName))
	{iFileHandle = FileCreate(txtFileName);FileClose(iFileHandle);}
	if((stream = fopen(txtFileName, "w+"))==NULL)
	{
		ShowMessage("File Open Failed!!!");
		return;
	}

	for(i=0;i<NUMV;i++)
	{
		fprintf(stream,"%4.2f\t",Vx[i]);
	}
	fprintf(stream, "\n");

	for(j=0;j<ngrid;j++)
	{
		fprintf(stream,"%d\t %d\t %d\t %d",j,Land1[j],Land2[j],Land2_cal[j]);
		fprintf(stream, "\n");
	}
	//Table1->Post();
	fclose(stream);

	delete[] FitForest;
	delete[] FitGrass;
	delete[] FitCrop;
	delete[] FitUrban;
	delete[] Land1;
	delete[] Land2;
	delete[] Land2_cal;


	RunEnd = Now(); time_End = time(NULL);
	SpendTime = difftime(time_End,time_Start);
	strtemp = IntToStr(SpendTime/3600) + ":" +
		IntToStr((SpendTime - SpendTime/3600*3600)/60) + ":" +
		IntToStr(SpendTime - SpendTime/3600*3600 - (SpendTime - SpendTime/3600*3600)/60*60);
	Edit1->Text = "目标函数VOBF = " + FloatToStrF(VOBF,ffFixed,4,2) + "; 耗时：" + strtemp; //FloatToStrF(FPoint[1].x,ffFixed,4,1)

	ShowMessage("Simulation Completed, VOBF = " + (String)VOBF);

}
//---------------------------------------------------------------------------
extern double __fastcall LandFit(int ngrid,int nLand,double ITER[],int LandID[],int *Landnum,
								 double FitForest[],double FitGrass[],double FitCrop[],double FitUrban[],
								 int Land1[],int Land2[],int Land2_cal[])
{//compute Value of Objective Function
	double VOF,maxFitness;
	//int LandID[nLand] = {1,2,3,5,6}; //nLand = 5; Crop,Forest,Crop,Urban,noUse
	//int Landnum[nLand] ,上面每种LUCC类型的栅格数目，代表面积
	double *Fitness = new double[nLand];
	double *gridFit = new double[ngrid];
	//double *gridFit2 = new double[ngrid];
	//int *gridid2 = new int[ngrid];
	int i,j,k,nk;
	for(i=0;i<ngrid;i++)
	{
		Fitness[0] = FitCrop[i] + ITER[0];
		Fitness[1] = FitForest[i] + ITER[1];
		Fitness[2] = FitGrass[i] + ITER[2];
		Fitness[3] = FitUrban[i] + ITER[3];
		maxFitness = MaxValue(Fitness,nLand - 2);
		gridFit[i] = maxFitness;
		for(j=0;j<nLand - 1;j++)
		{
			if(maxFitness==Fitness[j])
				Land2_cal[i] = LandID[j];
		}

	}//end for

	for(j=0;j<nLand - 1;j++)
	{nk=0;
	for(i=0;i<ngrid;i++)//找出每种LUcc的栅格
	{
		if(Land2_cal[i] == LandID[j])
		{

			// gridFit2[nk] = gridFit[i];
			// gridid2[nk] = i; //该种Lucc的栅格中第k个是整个流域的第i个
			nk++;
		} //end if
	} //end for

	if(nk<Landnum[j])
	{
		return 1;
		// goto Labelgogo1; //该种Lucc数据不够，目标函数值为1
	}

	double *gridFit2 = new double[nk];
	int *gridid2 = new int[nk];
	int *gridFit2i = new int[nk];

	k=0;
	for(i=0;i<ngrid;i++)//找出每种LUcc的栅格
	{
		if(Land2_cal[i] == LandID[j])
		{

			gridFit2[k] = gridFit[i];
			gridid2[k] = i; //该种Lucc的栅格中第k个是整个流域的第i个
			k++;
		} //end if
	} //end for

	sortASC(nk,gridFit2,gridFit2i);

	for(i=0;i<k - Landnum[j];i++)
	{
		Land2_cal[gridid2[gridFit2i[i]]] = LandID[nLand - 1]; //适应度较低的栅格土地利用类型设为noUse;
	}
	delete[] gridFit2i;
	delete[] gridFit2;
	delete[] gridid2;

	} //for(j=0;j<nLand - 1;j++)

	k=0;
	for(i=0;i<ngrid;i++)
	{
		if(Land2_cal[i] == Land2[i]) k++;
	}

	VOF = 1 - k*1.0/ngrid;//错误率 ，优化最小值
	//Labelgogo1:

	delete[] Fitness;
	delete[] gridFit;
	//delete[] gridFit2;
	//delete[] gridid2;
	return VOF;
}

//==============================================================================
//==============================================================================
//==============================================================================
/*
void __fastcall LinearInterp(struct TFPoint FPoint[])
{//根据FPoint[1]、FPoint[2]以及FPoint[0].x, 计算FPoint[0].y
if((FPoint[2].x - FPoint[1].x)!=0)
FPoint[0].y = FPoint[1].y +
(FPoint[0].x - FPoint[1].x)*
(FPoint[2].y - FPoint[1].y)/
(FPoint[2].x - FPoint[1].x);
else
FPoint[0].y = 0.5*(FPoint[2].y + FPoint[1].y);

} //TMDIChild::LinearInterp


extern int __fastcall wgsMonthsBetween(TDate dateBegin,TDate dateEnd)
{//计算两个日期之间的月数，包括开始和结束的月份，如1989-9-3与1990-6-9之间有10个月
int imonths;
if(dateBegin > dateEnd)
{ShowMessage(DateToStr(dateBegin)+" > "+ DateToStr(dateEnd));
return 0;}
if(YearOf(dateBegin) == YearOf(dateEnd))
imonths = MonthOf(dateEnd) - MonthOf(dateBegin) + 1;
else
imonths = (12 - MonthOf(dateBegin) + 1) + (MonthOf(dateEnd))
+((YearOf(dateEnd) - YearOf(dateBegin) - 1)*12);
return imonths;

}//TMDIChild::wgsMonthBetween

//---------------------------------------------------------------------------

AnsiString __fastcall IntToStrF(int iNum, int iLength)
{
AnsiString sRet;
int iLen;
//sRet = Trim(IntToStr(iNum));
iLen = (Trim(IntToStr(iNum))).Length();
for(int i=0;i<(iLength - iLen);i++)
sRet = sRet + "0";
sRet = sRet + Trim(IntToStr(iNum));

return sRet;
}

//---------------------------------------------------------------------------

extern double __fastcall ETatoETp(double PtoETp,double BagrovN,int nBagrovN,double *ArrayofBagrovN)
{//PtoETp=[0,5], Compute ETa/ETp according to P/ETp and BagrovN
int i,NumofArray;
double ETatoETp;
struct TFPoint *FPoint = new struct TFPoint[3];
if(PtoETp > 5.0)
PtoETp = 5.0;
else if(PtoETp < 0)
PtoETp = 0.0;
FPoint[0].x = BagrovN;
//ReadArrayofBagrovN(ArrayofBagrovN);
//NumofArray = sizeof(ArrayofBagrovN)/sizeof(ArrayofBagrovN[0]);
//ShowMessage(String(sizeof(ArrayofBagrovN))+":"+String(sizeof(ArrayofBagrovN[0]))+":"+String(NumofArray));

for(i=0;i<nBagrovN - 1;i++)
{
if((BagrovN >= ArrayofBagrovN[i])&&(BagrovN < ArrayofBagrovN[i + 1]))
{ FPoint[1].x = RoundTo(ArrayofBagrovN[i],-1);
FPoint[2].x = RoundTo(ArrayofBagrovN[i + 1],-1);
goto Label_GoTo;
}
}
Label_GoTo:
FPoint[1].y = FuncETatoETp(PtoETp,FPoint[1].x);
FPoint[2].y = FuncETatoETp(PtoETp,FPoint[2].x);

LinearInterp(FPoint);
ETatoETp = FPoint[0].y;
delete[] FPoint;
return ETatoETp;
}

//---------------------------------------------------------------------------

extern double __fastcall FuncETatoETp(double PtoETp,double BagrovN)
{//Bagrov方法的多项式拟合
double ETatoETp;
if(BagrovN==0.1)
ETatoETp = -0.0008*Power(PtoETp,4) + 0.0112*Power(PtoETp,3) - 0.0629*Power(PtoETp,2) + 0.2463*PtoETp + 0.0131;
else if(BagrovN==0.2)
ETatoETp = -0.0015*Power(PtoETp,4) + 0.0204*Power(PtoETp,3) - 0.1127*Power(PtoETp,2) + 0.3871*PtoETp + 0.0184;
else if(BagrovN==0.3)
ETatoETp = -0.0020*Power(PtoETp,4) + 0.0278*Power(PtoETp,3) - 0.1537*Power(PtoETp,2) + 0.4924*PtoETp + 0.0211;
else if(BagrovN==0.5)
ETatoETp = -0.0029*Power(PtoETp,4) + 0.0398*Power(PtoETp,3) - 0.2208*Power(PtoETp,2) + 0.6492*PtoETp + 0.0229;
else if(BagrovN==0.7)
ETatoETp = -0.0035*Power(PtoETp,4) + 0.0497*Power(PtoETp,3) - 0.2764*Power(PtoETp,2) + 0.7674*PtoETp + 0.0209;
else if(BagrovN==1.0)
ETatoETp = -0.0044*Power(PtoETp,4) + 0.0632*Power(PtoETp,3) - 0.3483*Power(PtoETp,2) + 0.9054*PtoETp + 0.0147;
else if(BagrovN==1.6)
ETatoETp = 0.0005*Power(PtoETp,6) - 0.0067*Power(PtoETp,5) + 0.0292*Power(PtoETp,4) + 0.0037*Power(PtoETp,3) - 0.3779*Power(PtoETp,2) + 1.0707*PtoETp + 0.0008;
else if(BagrovN==2.0)
ETatoETp = 0.0054*Power(PtoETp,6) - 0.0637*Power(PtoETp,5) + 0.2794*Power(PtoETp,4) - 0.5060*Power(PtoETp,3) + 0.0732*Power(PtoETp,2) + 0.9671*PtoETp + 0.0066;
else if(BagrovN==3.0)
ETatoETp = 0.0136*Power(PtoETp,6) - 0.1524*Power(PtoETp,5) + 0.6488*Power(PtoETp,4) - 1.2287*Power(PtoETp,3) + 0.6853*Power(PtoETp,2) + 0.8450*PtoETp + 0.0104;
else if(BagrovN==5.0)
ETatoETp = -0.1087*Power(PtoETp,6) + 0.6090*Power(PtoETp,5) - 1.0460*Power(PtoETp,4) + 0.3317*Power(PtoETp,3) + 0.1869*Power(PtoETp,2) + 0.8984*PtoETp + 0.0098;
else if(BagrovN==8.0)
ETatoETp = -0.3027*Power(PtoETp,6) + 1.8127*Power(PtoETp,5) - 3.7628*Power(PtoETp,4) + 3.0122*Power(PtoETp,3) - 0.8961*Power(PtoETp,2) + 1.0469*PtoETp + 0.0075;

if(ETatoETp > 1.0) ETatoETp = 1.0;
return ETatoETp;
}

//---------------------------------------------------------------------------

extern double __fastcall NashSutcliffeEC(int DataSize,double DataObs[],double DataCal[])
{//Nash-Sutcliffe Efficiency Criterion
double EC,MeanDataObs;
double sum1 = 0,sum2 = 0;
MeanDataObs = Mean(DataObs,DataSize - 1);
for(int i=0;i<DataSize;i++)
{
sum1 = sum1 + (DataObs[i] - MeanDataObs)*(DataObs[i] - MeanDataObs);
sum2 = sum2 + (DataObs[i] - DataCal[i])*(DataObs[i] - DataCal[i]);
}
EC = (sum1 = 0 ? 0:(1 - sum2/sum1));
return EC;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------


//=*****************************************************
* Function: DTVGM_Month Module. *
* Variable: None *
* Note: For different problem,user must change these *
* code. August 14,2003 *
*****************************************************=//

extern double __fastcall DTVGM_Month(int nBasinGrid,int ngrid,TDate dateBegin,TDate dateEnd,
String strDirRain,String strPrefix_rainfile,
int nBagrovN,double *ArrayofBagrovN,double *gridBagrovN,
int *gridSubID,double *gridArea,double *gridRCC,
double *Array_ETp,double *RunoffObs,
double *Parameter,double *Parameter_optimize,TProgressBar *ProgressBar,
double *Mean_gridrain,double *Mean_gridRunoff,double *Mean_gridSurfaceRunoff,double *Mean_gridSubsurfaceRunoff,
double *Mean_wateruse,double *Mean_gridETa,double *Mean_gridsm,double *RunoffCal,double &WB,double &NSEC)
{
//int nBasinGrid: Grids(subbasins) of Wbasin(the whole basin);
//int ngrid: subbasins of Obasin (the basin controled by an outlet);
//int *gridSubID: subbasin ID of Obasin in Wbasin;
//double *gridArea: Area of subbasin in Obasin
//double *gridRCC: Region Convert Coefficient of subbasin Runoff
//double *Mean_wateruse: Mean of subbasin wateruse in Obasin

int iyear,imonth,iyear0,imonth0,nMonths;
int i,j,k;
double AW0,AWC,g1,g2,Kr,KAW,PDc,PDc_basic,BasinArea,KET;//
double ETp,PtoETp,dlETatoETp,doublevalue;
double Weight_WB,ObjectFunction;
float ftemp;
AnsiString asFile;
FILE *stream;
char *saveFile;

g1 = Parameter_optimize[0];
g2 = Parameter_optimize[1];
Kr = Parameter_optimize[2];
KAW = Parameter_optimize[3];
AW0 = Parameter_optimize[4];

AWC = Parameter[0];
PDc = Parameter[1];
PDc_basic = Parameter[2];
KET = Parameter[3];
BasinArea = Parameter[4];
Weight_WB = Parameter[5]; //Weight_WaterBalance

nMonths = wgsMonthsBetween(dateBegin,dateEnd) ; //计算月数，MonthsBetween(2003-1-1,2004-1-1) = 11,实际上包括头尾有13个月
iyear = YearOf(dateBegin);
imonth = MonthOf(dateBegin) - 1;

float *gridrain = new float[nBasinGrid]; //必须定义成float类型，才能从文本文件中正确读取网格雨量
double *dlgridrain = new double[ngrid];
double *gridSurfaceRunoff = new double[ngrid];
double *gridSubsurfaceRunoff = new double[ngrid];
double *gridRunoff = new double[ngrid];
double *gridETa = new double[ngrid];
double *gridsm0 = new double[ngrid]; //时段初土壤湿度
double *gridsm = new double[ngrid]; //时段末土壤湿度
double *gridWateruse = new double[ngrid]; //Wateruse of subbasin
double *gridRunoffV = new double[ngrid]; //=gridRunoff*gridArea

float *gridETp = new float[nBasinGrid]; //2003-11-20必须定义成float类型，才能从文本文件中正确读取网格ETp
double *dlgridETp = new double[ngrid]; //2003-11-20

for(k=0;k<ngrid;k++)
gridsm[k] = AW0;

for(i=0;i<nMonths;i++) //处理iMonth个月的数据
{
imonth = imonth + 1;
if(imonth>12)
{iyear = iyear + 1;imonth = 1;}
//ETp = KET*Array_ETp[i]; //2003-11-20,Potential Evapotranspiration,KET为水面蒸发折算为潜在蒸发的系数
//Kr0 = 0.5*Kr + 0.25*ETp/AWC; //构造另一个参数
asFile = Trim(strDirRain + strPrefix_rainfile + IntToStr(iyear) + IntToStrF(imonth,2) + ".txt");//Open File: Gridrain

if(FileExists(asFile))
{
if((stream=fopen(asFile.c_str(),"rt"))==NULL)
{
fprintf(stderr,"Cannot open input file.\n");
return -100;
}
}
else
{
ShowMessage(AnsiString("Couldnot find the file:")+ asFile +".");
return -200;
}

for(k=0;k<nBasinGrid;k++) // !feof(stream)
{ fscanf(stream,"%d %d %d %f %f",&iyear0,&imonth0,&j,&gridrain[k],&gridETp[k]); // 2003-11-20,fscanf(stream,"\n");
}
fclose(stream);

for(k=0;k<ngrid;k++) // !feof(stream)
{dlgridrain[k] = gridrain[gridSubID[k]];
dlgridETp[k] = KET*gridETp[gridSubID[k]]; //2003-11-20,KET为水面蒸发折算为潜在蒸发的系数
gridsm0[k] = gridsm[k];
}

for(k=0;k<ngrid;k++)
{ //Runoff Generation
ETp = dlgridETp[k]; //2003-11-20
PtoETp = dlgridrain[k]/ETp;
doublevalue = gridBagrovN[k];
dlETatoETp = ETatoETp(PtoETp,doublevalue,nBagrovN,ArrayofBagrovN);
gridETa[k] = ETp*MIN((1 - KAW)*dlETatoETp + KAW*gridsm0[k]/AWC,1); // 2003-10-30 (1 - KAW)*

doublevalue = (dlgridrain[k]>PDc?(PDc/PDc_basic):1)*g1*Power(gridsm0[k]/AWC,g2); //Power(dlETatoETp + KAW*gridsm0[k]/AWC,g2)
gridSurfaceRunoff[k] = MIN(doublevalue,1)*dlgridrain[k];
doublevalue = (dlgridrain[k] - gridSurfaceRunoff[k] - gridETa[k] + (1 - 0.5*Kr)*gridsm0[k])/(1 + 0.5*Kr); //sm(t+1)
if(doublevalue>=0)
{if(doublevalue>AWC)
{gridsm[k] = AWC;
gridSubsurfaceRunoff[k] = 0.5*Kr*(gridsm0[k] + gridsm[k]);
ftemp = gridsm0[k] + dlgridrain[k] - gridSubsurfaceRunoff[k] - gridSurfaceRunoff[k] - gridsm[k];
gridETa[k]= MIN(ftemp,ETp);
gridSurfaceRunoff[k] = gridsm0[k] + dlgridrain[k] - gridSubsurfaceRunoff[k] - gridETa[k] - gridsm[k];
}
else //doublevalue = [0,AWC]
{gridsm[k] = doublevalue;
gridSubsurfaceRunoff[k] = 0.5*Kr*(gridsm0[k] + gridsm[k]);}
}
else //doublevalue < 0
{if((gridsm0[k] + dlgridrain[k] - gridSurfaceRunoff[k] - gridETa[k])<0)
{gridSubsurfaceRunoff[k] = 0;gridETa[k] = gridsm0[k] + dlgridrain[k] - gridSurfaceRunoff[k];}
else
{gridSubsurfaceRunoff[k] = gridsm0[k] + dlgridrain[k] - gridSurfaceRunoff[k] - gridETa[k];}
gridsm[k] = 0;
}


gridRunoff[k] = (gridSurfaceRunoff[k] + gridSubsurfaceRunoff[k])*gridRCC[k];
gridRunoffV[k] = gridRunoff[k]*gridArea[k];
gridWateruse[k] = gridSurfaceRunoff[k] + gridSubsurfaceRunoff[k] - gridRunoff[k];

//Save Grid Data
asFile = Trim(strDirRain + "GData\\GR" + IntToStr(iyear) + IntToStrF(imonth,2) + ".dat");
saveFile = asFile.c_str();
SaveGridData(blVar,saveFile,ngrid, gridSubID,dlgridrain, gridETa, gridsm, gridWateruse,gridRunoff, gridSurfaceRunoff,gridSubsurfaceRunoff, gridSubsurfaceRunoff);
} //k
RunoffCal[i] = Sum(gridRunoffV,ngrid - 1)/(3*24*36); //mm->m^3/s
Mean_gridrain[i] = Mean(dlgridrain,ngrid - 1);
Mean_gridRunoff[i] = Mean(gridRunoff,ngrid - 1);
Mean_gridSurfaceRunoff[i] = Mean(gridSurfaceRunoff,ngrid - 1);
Mean_gridSubsurfaceRunoff[i] = Mean(gridSubsurfaceRunoff,ngrid - 1);
Mean_gridETa[i] = Mean(gridETa,ngrid - 1);
Mean_gridsm[i] = Mean(gridsm,ngrid - 1);
Mean_wateruse[i] = Mean(gridWateruse,ngrid - 1);
ProgressBar->Position = 100.0*i/nMonths;
}//i

//=*********************************************************
//=ATTENTION: 计算数组的平均值， *
//=用函数Mean(const double * Data, const int Data_Size)时, *
//=Data_Size应该是【数组元素个数 － 1】 ×
//=必须是double类型数组 *
//=*******************************************************=//
WB = Sum(RunoffCal,nMonths - 1)/Sum(RunoffObs,nMonths - 1);
NSEC = NashSutcliffeEC(nMonths,RunoffObs,RunoffCal);
ObjectFunction = Weight_WB*fabs(1.0 - WB) + (1.0 - Weight_WB)*fabs(0.8 - NSEC);

delete[] gridrain;
delete[] gridSurfaceRunoff;
delete[] gridSubsurfaceRunoff;
delete[] gridRunoff;
delete[] gridETa;
delete[] gridsm0;
delete[] gridsm;
delete[] dlgridrain;
delete[] gridWateruse;
delete[] gridRunoffV;

delete[] gridETp;
delete[] dlgridETp;

return ObjectFunction;
} //End DTVGM_Month

//---------------------------------------------------------------------------

void __fastcall SaveGridData(bool blSave, char *txtFileName,
int ngrid, int *gridID,double *gridP, double *gridETa, double *gridSM,
double *gridWU,double *gridR, double *gridRS, double *gridRSS, double *gridRG)
{
//gridRS: Grid Surface Runoff
//gridRSS: grid SubSurface Runoff
//gridRG: grid Gound Runoff
//gridWU: grid Water Use
if(blSave == false) return;
FILE *stream;
int iFileHandle;
if(!FileExists(txtFileName))
{ iFileHandle = FileCreate(txtFileName);FileClose(iFileHandle);}
if((stream = fopen(txtFileName, "w+"))==NULL)
{
ShowMessage("File Open Failed!!!");
return;
}
fprintf(stream, "%s\t %s\t %s\t %s\t %s\t %s\t %s\t %s\t %s\t",
"SubBasin","gridP","gridETa","gridSM","gridWU","gridR","gridRS","gridRSS","gridRG");
fprintf(stream, "\n");
for(int j=0;j<ngrid;j++)
{
fprintf(stream,"%d\t %6.2f\t %6.2f\t %6.2f\t %6.2f\t %6.2f\t %6.2f\t %6.2f\t %6.2f\t",
gridID[j],gridP[j],gridETa[j],gridSM[j],gridWU[j],gridR[j],gridRS[j],gridRSS[j],gridRG[j]);
fprintf(stream, "\n");
}

fclose(stream);
return;
}
*/
//==============================================================================
//==============================================================================
//==============================================================================
/*
void TMDIChild::display(char **data,int m,int n)
{
for (int i = 0; i < m; i++)
{
ShowMessage(data[i]);
// for (int j = 0; j < n; j++)
// cout << data[i][j] << " ";
// cout << "\n" << endl;
}
}

void TMDIChild::de_allocate(char **data,int m)
{
for (int i = 0; i < m; i++)
delete[] data[i]; // STEP 1: DELETE THE COLUMNS

delete[] data; // STEP 2: DELETE THE ROWS

}

*/
//void TMDIChild::ShowProgressBar(long RecordCount,long CurrentRecord){}

