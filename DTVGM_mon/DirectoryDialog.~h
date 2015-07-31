//----------------------------------------------------------------------------
#ifndef DirectoryDialogH
#define DirectoryDialogH
//----------------------------------------------------------------------------
#include <vcl\System.hpp>
#include <vcl\Windows.hpp>
#include <vcl\SysUtils.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\ExtCtrls.hpp>
#include <FileCtrl.hpp>
//----------------------------------------------------------------------------
class TDirectoryDlg : public TForm
{
__published:        
	TButton *OKBtn;
	TButton *CancelBtn;
	TBevel *Bevel1;
        TDirectoryListBox *DirectoryListBox1;
        TDriveComboBox *DriveComboBox1;
        TLabel *Label1;
        TLabel *Label_dir;
        TFileListBox *FileListBox1;
        TEdit *EditFile;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TFilterComboBox *FilterComboBox1;
        void __fastcall OKBtnClick(TObject *Sender);
        void __fastcall DirectoryListBox1Change(TObject *Sender);
private:
public:
	virtual __fastcall TDirectoryDlg(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TDirectoryDlg *DirectoryDlg;
//----------------------------------------------------------------------------
#endif    
