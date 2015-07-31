//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------
USEFORM("main.cpp", MainForm);
USEFORM("childwin.cpp", MDIChild);
USEFORM("about.cpp", AboutBox);
USEFORM("QueryDemo.cpp", FormQuery);
USEFORM("DateDialog.cpp", DateDlg);
USEFORM("DM1.cpp", DM); /* TDataModule: File Type */
USEFORM("workdir.cpp", WorkDirDlg);
USEFORM("DirectoryDialog.cpp", DirectoryDlg);
USEFORM("ModelParaDialog.cpp", ModelParaDlg);
USEFORM("HydroGraph.cpp", frmHydroGraph);
USEFORM("TopIndexFiles.cpp", DlgTOPIndex);
USEFORM("ChartAxisX.cpp", DlgChartEdit);
USEFORM("MPSAmap.cpp", frmMPSAmap);
USEFORM("C:\PROGRAM FILES\BORLAND\CBUILDER6\OBJREPOS\okcancl1.cpp", OKBottomDlg);
USEFORM("MCMCINI.cpp", DlgMCMCINI);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Application->Initialize();
	Application->CreateForm(__classid(TMainForm), &MainForm);
                 Application->CreateForm(__classid(TAboutBox), &AboutBox);
                 Application->CreateForm(__classid(TDateDlg), &DateDlg);
                 Application->CreateForm(__classid(TDM), &DM);
                 Application->CreateForm(__classid(TDirectoryDlg), &DirectoryDlg);
                 Application->CreateForm(__classid(TWorkDirDlg), &WorkDirDlg);
                 Application->CreateForm(__classid(TModelParaDlg), &ModelParaDlg);
                 Application->CreateForm(__classid(TDlgTOPIndex), &DlgTOPIndex);
                 Application->CreateForm(__classid(TDlgChartEdit), &DlgChartEdit);
                 Application->CreateForm(__classid(TfrmMPSAmap), &frmMPSAmap);
                 Application->CreateForm(__classid(TDlgMCMCINI), &DlgMCMCINI);
                 Application->Run();


	return 0;
}
//---------------------------------------------------------------------


