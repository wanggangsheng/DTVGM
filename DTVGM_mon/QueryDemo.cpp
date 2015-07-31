//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "QueryDemo.h"
#include "main.h"
#include <dir.h>
#include "DM1.h"
//#include "struct.h"
#include "DTVGM.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormQuery *FormQuery;
//---------------------------------------------------------------------------
__fastcall TFormQuery::TFormQuery(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormQuery::FormCreate(TObject *Sender)
{
        DM->ADOConnection1->Connected = true;
        //DM->ADOTable0->TableName = "dataquery";
        //DM->DS0->DataSet = DM->ADOTable0;
        //DBGrid1->DataSource = DM->DS0;
        //DM->ADOTable0->Open();
        gstrAlias = DM->ADOConnection1->DefaultDatabase;
        Screen->Cursor = crHourGlass;
        PageControl1->ActivePage = TabSheet1;
        ListBox1->Items->Clear();
        Session->GetAliasNames(ListBox1->Items);

        Screen->Cursor=crDefault;
        if(ListBox1->Items->Count<1)
                MessageDlg("There are no database aliases currently defined.You need at least one alias to use this demonstration.",
                mtError, TMsgDlgButtons() << mbOK, 0);

        //Table2->Database->LoginPrompt=false;
        //DeleteAllRecords(Table2);

}

//---------------------------------------------------------------------------



void __fastcall TFormQuery::FormShow(TObject *Sender)
{
//本project中只连接数据库wgsdhm
//TDatabase *DB = Session->FindDatabase(gstrAlias);
//if( !DB )                               // Database does not exist for session so
//        DB = Session->OpenDatabase(gstrAlias); // create and open it
//else
        DM->ADOConnection1->GetTableNames(ListBox2->Items,false);   //target for table list
        ListBox1->Enabled=false;
        if(DM->ADOTable0->Active)
        {       DM->ADOTable0->Filtered = false;
                DM->ADOTable0->Close();
        }
        DM->ADOTable0->TableName = "dataquery";
        DM->DS0->DataSet = DM->ADOTable0;
        DBGrid1->DataSource = DM->DS0;
        DM->ADOTable0->Active = true;
        //DeleteAllRecords(DM->ADOTable0);
}
//---------------------------------------------------------------------------
void __fastcall TFormQuery::ListBox1Click(TObject *Sender)
{
String strValue;      //选择的数据库别名
Boolean bIsLocal;     //判断是否为本地库
//TStringList *slParams = new TStringList();; //存储选择数据库的参数
Integer iCounter;     //

strValue = ListBox1->Items->Strings[ListBox1->ItemIndex];

ListBox2->Items->Clear();

/*int i=Session->DatabaseCount;
if(i>0)
{
//strValue=Session->Databases[i]->AliasName;
Session->Databases[i-1]->Connected=false;
Session->Databases[i-1]->AliasName=strValue;
Session->Databases[i-1]->LoginPrompt=false;
Session->Databases[i-1]->Connected=true; } */

        Session->GetTableNames(strValue,
                       "",     //Pattern to match
                       CheckBox1->Checked, //show extensions flag
                       false, //CheckBox2->Checkedshow system tables flag
                       ListBox2->Items);   //target for table list


Screen->Cursor=crDefault;
if(ListBox2->Items->Count<1)
        MessageDlg("There are no TABLEs in the alias you selected.Please choose another",mtError,TMsgDlgButtons() << mbOK, 0);

ListBox3->Items->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TFormQuery::ListBox2Click(TObject *Sender)
{
////if(MessageDlg("Delete existed Query or Filter?",mtConfirmation,TMsgDlgButtons() <<mbNo<< mbYes,0)==mrYes)
DeleteAllRecords(DM->ADOTable0);
Screen->Cursor=crHourGlass;
DM->ADOTable2->Filtered = false;
try
{
  if(DM->ADOTable2->Active)
  {     
        DM->ADOTable2->Close();
  }
  // fill a list box with alias names for the user to select from
  DM->ADOTable2->TableName=ListBox2->Items->Strings[ListBox2->ItemIndex];

  DM->ADOTable2->Open();
  DBGrid1->Enabled = true;
  if(DM->ADOTable2->Active)
        {DM->ADOTable2->GetFieldNames(ListBox3->Items);
         DM->ADOTable2->GetFieldNames(DBGrid1->Columns->Items[1]->PickList);  //items[1]表示第2个字段 fieldname
         if(DM->ADOTable0->Active)
         {DM->ADOTable0->Insert();
          DM->ADOTable0->First();
          DM->ADOTable0->Edit();
          DM->ADOTable0->FieldValues["QueryID"] = 1;
         }}
}
__finally
{Screen->Cursor=crDefault;}
}
//---------------------------------------------------------------------------


void __fastcall TFormQuery::BitBtn1Click(TObject *Sender)
{//Query
if(BitBtn1->Caption == "Set Filter")
        {TFormQuery::BitBtn3Click(Sender);
         return;
        }
String strAlias,strTable,strField,strOperator,strValue,strLogical,strWhere,strQuote,strQuery;
int iOperator;
TFieldType FDType;
enum etSQLOps {soNoCondition, //0 no field conditions:no WHERE clause
               soEqual,       //1 equals:=
               soNotEqual,    //2 is not equal to:<>
               soLessThan,    //3 is less than:<
               soLessEqual,   //4 is less than or equal to:<=
               soMoreThan,    //5 is greater than:>
               soMoreEqual,   //6 is greater than or equal to:>=
               soBetween,     //7 Between ... and ...
               soStartsWith,  //8 starts with:LIKE xx%
               soNoStartsWith,//9 doesn't start with:NOT LIKE xx%
               soEndsWith,    //10 ends with:LIKE %xx
               soNoEndsWith,  //11 doesn't end with:NOT LIKE %xx
               soContains,    //12 contains:LIKE %xx%
               soNoContains,  //13 doesn't contain:NOT LIKE %xx%
               soNull,       //14 is null:
               soNotNull,    //15 is not null:
               soInside,      //16 contain only:IN(xx,yy,zz)
               soOutside};    //17 doesn't contain:NOT IN(xx,yy,zz)


if(ListBox2->ItemIndex==-1)
        {ShowMessage("Cannot Run Query: No Table Selected");
        return;}
else
        strTable=ListBox2->Items->Strings[ListBox2->ItemIndex];

//Table2->Post();        //不能写这一句，post之后Table2将处于“不可编辑”状态

//strWhere="";
DM->ADOTable0->First();
if(DM->ADOTable0->RecordCount>0)
{       while(!DM->ADOTable0->Eof)
        //for(int i=0;i<DM->ADOTable0->RecordCount;i++)
                {strField = DM->ADOTable0->Fields->FieldByName("FieldName")->AsString;
                 strOperator = DM->ADOTable0->Fields->FieldByName("Operator")->AsString;
                 strValue = DM->ADOTable0->Fields->FieldByName("FieldValue")->AsString;
                 strLogical = DM->ADOTable0->Fields->FieldByName("Logical")->AsString;
                 if(strLogical!="") strLogical = " " + strLogical + " ";
                 iOperator = DBGrid1->Columns->Items[2]->PickList->IndexOf(strOperator); //Items[2]表示第3个字段 Operator

                 if(strField!="" && strField!=NULL)
                 {
                         FDType = DM->ADOTable2->FieldByName(strField)->DataType;
                         {if(FDType==ftString||FDType==ftWideString||FDType==ftMemo)
                                strQuote="'"; //ftString和ftMemo类型的值要加引号
                          else if(FDType==ftDate||FDType==ftDateTime)
                                strQuote = "#";  //Access数据库查询，对日期型要求“＃”
                          else
                                //strQuote="'";}  //SQL Server数据库，实际运行发现(if_else)没有必要
                                strQuote=" ";} //Access数据库不能要引号
                 } //end if(strField!="" && strField!=NULL)
                 switch(iOperator)
                {
                        case soNoCondition: strWhere="";break;
                        case soEqual: strWhere = strWhere + strField + " = " + strQuote + strValue + strQuote + strLogical;break;
                        case soNotEqual: strWhere = strWhere + strField + " <> " + strQuote + strValue + strQuote + strLogical;break;
                        case soLessThan: strWhere = strWhere + strField + " < " + strQuote + strValue + strQuote + strLogical;break;
                        case soLessEqual: strWhere = strWhere + strField + " <= " + strQuote + strValue + strQuote + strLogical;break;
                        case soMoreThan: strWhere = strWhere + strField + " > " + strQuote + strValue + strQuote + strLogical;break;
                        case soMoreEqual: strWhere = strWhere + strField + " >= " + strQuote + strValue + strQuote + strLogical;break;
                        case soBetween: strWhere=strWhere +strField + " BETWEEN " + strValue + " " + strLogical;break;
                        case soStartsWith: strWhere = strWhere + strField + " LIKE " + strQuote + strValue + "%" + strQuote + strLogical;break;
                        case soNoStartsWith: strWhere = strWhere + strField + " NOT LIKE " + strQuote + strValue + "%" + strQuote + strLogical;break;
                        case soEndsWith: strWhere = strWhere + strField + " LIKE " + strQuote + "%" + strValue + strQuote + strLogical;break;
                        case soNoEndsWith: strWhere = strWhere + strField + " NOT LIKE " + strQuote + "%" + strValue + strQuote + strLogical;break;
                        case soContains: strWhere = strWhere + strField + " LIKE " + strQuote + "%" + strValue + "%" + strQuote + strLogical;break;
                        case soNoContains: strWhere = strWhere + strField + " NOT LIKE " + strQuote + "%" + strValue + "%" + strQuote + strLogical;break;
                        case soNull:strWhere=strWhere + strField + " IS NULL ";break;    //break很重要，否则会继续转向下一个case
                        case soNotNull:strWhere=strWhere + strField + " IS NOT NULL ";break;
                        case soInside:strWhere=strWhere +strField + " IN (" + strValue + ") " + strLogical;break;
                        case soOutside:strWhere=strWhere +strField + " NOT IN (" + strValue + ") " + strLogical;break;
                        default: strWhere="";break;
                } //switch

                 DM->ADOTable0->Next();
                }//end for
        if(strField==""||iOperator<=soNoCondition||strValue=="")
                strQuery="SELECT * FROM " + strTable;
        else
                strQuery="SELECT * FROM " + strTable + " WHERE (" + strWhere+")";
        RichEdit1->Lines->Add(strQuery);
        MainForm->TMainForm::RunQuery(true,gstrAlias,strTable,strField,strQuery);
} //endif
} //end TFormQuery::BitBtn1Click

//---------------------------------------------------------------------------


void __fastcall TFormQuery::Table2NewRecord(TDataSet *DataSet)
{
       /* int iqueryid;
        if(!Table2->Bof)
        {Table2->FindPrior();
        iqueryid = Table2->Fields->FieldByName("QueryID")->AsInteger;
        Table2->FindNext();
        Table2->Fields->Fields[0]->AsInteger = iqueryid + 1;
        //Table2->Fields->FieldByName("QueryID")->AsInteger = iqueryid + 1;
        } */
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

void __fastcall TFormQuery::DBGrid1CellClick(TColumn *Column)
{
if((DBGrid1->Columns->Items[1]->PickList->Count<1)&&(Column->FieldName == "FieldName")) //Items[1]:FiledName
        {ShowMessage("No Table was Selected!");
         return;}
}
//---------------------------------------------------------------------------



void __fastcall TFormQuery::Append1Click(TObject *Sender)
{
        int iqueryid;
        DM->ADOTable0->FindLast();
        iqueryid = DM->ADOTable0->FieldValues["QueryID"];
        DM->ADOTable0->Append();
        DM->ADOTable0->FieldValues["QueryID"] = iqueryid + 1;
        DM->ADOTable0->Post();

}
//---------------------------------------------------------------------------


void __fastcall TFormQuery::Delete1Click(TObject *Sender)
{
        DM->ADOTable0->Delete();
        DM->ADOTable0->First();
        for(int i=0;i<DM->ADOTable0->RecordCount;i++)//从第1条记录开始重新编号
        {
        DM->ADOTable0->Edit();  //设置为编辑状态
        DM->ADOTable0->FieldValues["QueryID"] = i + 1;
        DM->ADOTable0->Next();
        }
        //Table2->Post();
}
//---------------------------------------------------------------------------


void __fastcall TFormQuery::DeleteAll1Click(TObject *Sender)
{
        switch(MessageDlg("删除所有记录吗?",mtConfirmation,TMsgDlgButtons() << mbYes<<mbNo,0))
        {case mrYes:
                DeleteAllRecords(DM->ADOTable0);
                break;
         case mrNo:break;}
}
//---------------------------------------------------------------------------

void __fastcall TFormQuery::BitBtn3Click(TObject *Sender)
{//Filter
String strAlias,strTable,strField,strOperator,strValue,strLogical,strQuote,strFilter;
int iOperator;
TFieldType FDType;
enum etSQLOps {soNoCondition, //no field conditions:no WHERE clause
               soEqual,       //equals:=
               soNotEqual,    //is not equal to:<>
               soLessThan,    //is less than:<
               soLessEqual,   //is less than or equal to:<=
               soMoreThan,    //is greater than:>
               soMoreEqual,   //is greater than or equal to:>=
               soBetween,     //7 Between ... and ...
               soStartsWith,  //starts with:LIKE xx%
               soNoStartsWith,//doesn't start with:NOT LIKE xx%
               soEndsWith,    //ends with:LIKE %xx
               soNoEndsWith,  //doesn't end with:NOT LIKE %xx
               soContains,    //contains:LIKE %xx%
               soNoContains,  //doesn't contain:NOT LIKE %xx%
               soNull,       //is null:
               soNotNull,    //is not null:
               soInside,      //contain only:IN(xx,yy,zz)
               soOutside};    //doesn't contain:NOT IN(xx,yy,zz)

if(ListBox2->ItemIndex==-1)
        {ShowMessage("Cannot Run Filter: No Table Selected");
        return;}
else
        strTable=ListBox2->Items->Strings[ListBox2->ItemIndex];

DM->ADOTable0->First();
if(DM->ADOTable0->RecordCount>0)
{       for(int i=0;i<DM->ADOTable0->RecordCount;i++)
                {strField = DM->ADOTable0->Fields->FieldByName("FieldName")->AsString;
                 strOperator = DM->ADOTable0->Fields->FieldByName("Operator")->AsString;
                 strValue = DM->ADOTable0->Fields->FieldByName("FieldValue")->AsString;
                 strLogical = DM->ADOTable0->Fields->FieldByName("Logical")->AsString;
                 if(strLogical!="") strLogical = " " + strLogical + " ";
                 iOperator = DBGrid1->Columns->Items[2]->PickList->IndexOf(strOperator); //Items[2]表示第3个字段 Operator

                 if(strField!="" && strField!=NULL)
                 {       FDType = DM->ADOTable2->FieldByName(strField)->DataType;
                         {if(FDType==ftString||FDType==ftWideString||FDType==ftMemo)
                                strQuote="'"; //ftString和ftMemo类型的值要加引号
                         else
                                //strQuote="'";}  //SQL Server数据库，实际运行发现(if_else)没有必要
                                strQuote=" ";} //Access数据库不能要引号
                 }
                 switch(iOperator)
                {
                        case soNoCondition: strFilter="";break;
                        case soEqual: strFilter = strFilter + strField + " = " + strQuote + strValue + strQuote + strLogical;break;
                        case soNotEqual: strFilter = strFilter + strField + " <> " + strQuote + strValue + strQuote + strLogical;break;
                        case soLessThan: strFilter = strFilter + strField + " < " + strQuote + strValue + strQuote + strLogical;break;
                        case soLessEqual: strFilter = strFilter + strField + " <= " + strQuote + strValue + strQuote + strLogical;break;
                        case soMoreThan: strFilter = strFilter + strField + " > " + strQuote + strValue + strQuote + strLogical;break;
                        case soMoreEqual: strFilter = strFilter + strField + " >= " + strQuote + strValue + strQuote + strLogical;break;
                       /* case soBetween: strFilter = strFilter +strField + " BETWEEN " + strValue + " " + strLogical;break;
                        case soStartsWith: strWhere = strWhere + strField + " LIKE " + strQuote + strValue + "%" + strQuote + strLogical;break;
                        case soNoStartsWith: strWhere = strWhere + strField + " NOT LIKE " + strQuote + strValue + "%" + strQuote + strLogical;break;
                        case soEndsWith: strWhere = strWhere + strField + " LIKE " + strQuote + "%" + strValue + strQuote + strLogical;break;
                        case soNoEndsWith: strWhere = strWhere + strField + " NOT LIKE " + strQuote + "%" + strValue + strQuote + strLogical;break;
                        case soContains: strWhere = strWhere + strField + " LIKE " + strQuote + "%" + strValue + "%" + strQuote + strLogical;break;
                        case soNoContains: strWhere = strWhere + strField + " NOT LIKE " + strQuote + "%" + strValue + "%" + strQuote + strLogical;break;
                        case soNull:strWhere=strWhere + strField + " IS NULL ";
                        case soNotNull:strWhere=strWhere + strField + " IS NOT NULL ";   */
                        default: strFilter="";break;
                } //switch

                 DM->ADOTable0->Next();
                }//end for
        RichEdit1->Lines->Add(strFilter);
        MainForm->TMainForm::SetFilter(true,gstrAlias,strTable,strField,strFilter);
} //endif
} //end SetFilter
//---------------------------------------------------------------------------

void __fastcall TFormQuery::ListBox3Click(TObject *Sender)
{
       DM->ADOTable0->Edit();
       DM->ADOTable0->FieldValues["FieldName"]=ListBox3->Items->Strings[ListBox3->ItemIndex];
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------


void __fastcall TFormQuery::BitBtn5Click(TObject *Sender)
{
        MainForm->OpenDialog->DefaultExt="sql;txt;rtf";
        //MainForm->OpenDialog->FileName="*sql;*.txt;*.rtf";
        MainForm->OpenDialog->Filter="SQL Files(*.sql)|*.SQL|Text Files(*.txt)|*.TXT|RTF Files(*.rtf)|*.RTF"; //
        MainForm->OpenDialog->InitialDir=Application->ExeName;
        MainForm->OpenDialog->Title="选择导入数据文件";
        MainForm->OpenDialog->Options.Clear();
        MainForm->OpenDialog->Options<<ofFileMustExist
                         <<ofHideReadOnly
                         <<ofNoChangeDir
                         <<ofCreatePrompt;
        if (MainForm->OpenDialog->Execute())
	        RichEdit1->Lines->LoadFromFile(MainForm->OpenDialog->FileName);
}
//---------------------------------------------------------------------------

void __fastcall TFormQuery::BitBtn6Click(TObject *Sender)
{
     int iFileHandle;
     char szFileName[MAXFILE+4]; //#include <dir.h>

     MainForm->SaveDialog->DefaultExt="sql;rft;txt";
     //MainForm->SaveDialog->FileName="*.rft;*.txt";
     MainForm->SaveDialog->Filter="SQL Files(*.sql)|*.SQL|Text Files(*.txt)|*.TXT|RTF Files(*.rtf)|*.RTF";
     MainForm->SaveDialog->InitialDir=Application->ExeName;
     MainForm->SaveDialog->Title="选择导出数据文件名";
     MainForm->SaveDialog->Options.Clear();
     MainForm->SaveDialog->Options<<ofFileMustExist
                         <<ofHideReadOnly
                         <<ofNoChangeDir;
     if(MainForm->SaveDialog->Execute())
     {
        if(FileExists(MainForm->SaveDialog->FileName))
        {
                ShowMessage(AnsiString("文件:")+MainForm->SaveDialog->FileName+"已经存在,扩展名已改为BAK.");
                fnsplit(MainForm->SaveDialog->FileName.c_str(), 0, 0, szFileName, 0);
                strcat(szFileName, ".BAK");
                RenameFile(MainForm->SaveDialog->FileName, szFileName);
        }
        //iFileHandle = FileCreate(MainForm->SaveDialog->FileName);
        RichEdit1->Lines->SaveToFile(MainForm->SaveDialog->FileName);
        //FileClose(iFileHandle);
     }
}
//---------------------------------------------------------------------------

void __fastcall TFormQuery::BitBtn7Click(TObject *Sender)
{
        RichEdit1->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TFormQuery::btnQueryClick(TObject *Sender)
{
        String strQuery;
        strQuery = RichEdit1->Text;
        MainForm->TMainForm::RunQuery(true,gstrAlias,NULL,NULL,strQuery);       
}
//---------------------------------------------------------------------------

