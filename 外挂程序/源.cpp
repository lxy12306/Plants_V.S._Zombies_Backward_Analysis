#include "F:\Code\object\WinUserHandle\MyWindows.h"
#include <windowsx.h>
#include "resource.h"
#include <tchar.h>
#include "F:\Code\object\ProcessAndHwndAndThread\Inject\MyInject.h"
#include "Messages.h"

///////////////////////////////////////////////////////////////////////////////
//全局变量
HWND g_hWndDll = NULL; //发送消息的句柄

///////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Dlg初始化的时候干的事情
/// </summary>
/// <param name="hwnd">窗口句柄</param>
/// <param name="hwndFocus">初始默认Focus的控件句柄</param>
/// <param name="lParam">参数</param>
/// <returns>是否初始化成功</returns>
BOOL Dlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) 
{

    return(TRUE);
}
//////////////////////////////////////////////////////////////////////////////
/// <summary>
/// 窗口控件的处理函数
/// </summary>
/// <param name="hwnd">窗口句柄</param>
/// <param name="id">控件ID</param>
/// <param name="hwndCtl">控件句柄</param>
/// <param name="codeNotify">默认为0</param>
void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) 
{
    switch (id) {

    case IDCANCEL:
    {
        SendMessage(g_hWndDll, UDM_INFINITE_SUNSHINE, FALSE, 0);
        SendMessage(g_hWndDll, UDM_NO_COOLING_TIME, FALSE, 0);
        SendMessage(g_hWndDll, WM_CLOSE, 0, 0);
        EndDialog(hwnd, id);
    }break;
    case IDSun:
    {
        char atcBuffer[0x10];
        GetWindowTextA(GetDlgItem(hwnd, IDC_EDITSun), atcBuffer, 0x10);
        DWORD dwSunLight = strtoul(atcBuffer, NULL, 10);//10进制转换
        SendMessage(g_hWndDll, UDM_AMOUNT_OF_SUNLIGHT, dwSunLight, 0);
    }break;
    case IDSunInf:
    {
        SendMessage(g_hWndDll, UDM_INFINITE_SUNSHINE, TRUE, 0);
        EnableWindow(GetDlgItem(hwnd, IDSunInf), false);
        EnableWindow(GetDlgItem(hwnd, IDSunNotInf), true);
    }break;
    case IDSunNotInf:
    {
        SendMessage(g_hWndDll, UDM_INFINITE_SUNSHINE, FALSE, 0);
        EnableWindow(GetDlgItem(hwnd, IDSunNotInf), false);
        EnableWindow(GetDlgItem(hwnd, IDSunInf), true);
    }break;

    case IDNoCoolingTime:
    {
        SendMessage(g_hWndDll, UDM_NO_COOLING_TIME, TRUE, 0);
        EnableWindow(GetDlgItem(hwnd, IDNoCoolingTime), false);
        EnableWindow(GetDlgItem(hwnd, IDCoolingTime), true);
    }break;
    case IDCoolingTime:
    {
        SendMessage(g_hWndDll, UDM_NO_COOLING_TIME, FALSE, 0);
        EnableWindow(GetDlgItem(hwnd, IDCoolingTime), false);
        EnableWindow(GetDlgItem(hwnd, IDNoCoolingTime), true);
    }break;
    case IDKillZoobie:
    {
        SendMessage(g_hWndDll, UDM_ZOMBIE_TAKEDOWN, TRUE, 0);
        EnableWindow(GetDlgItem(hwnd, IDKillZoobie), false);
        EnableWindow(GetDlgItem(hwnd, IDNoKillZoobe), true);
    }break;
    case IDNoKillZoobe:
    {
        SendMessage(g_hWndDll, UDM_ZOMBIE_TAKEDOWN, FALSE, 0);
        EnableWindow(GetDlgItem(hwnd, IDNoKillZoobe), false);
        EnableWindow(GetDlgItem(hwnd, IDKillZoobie), true);
    }break;
    case IDZiDanFengSheng:
    {
        SendMessage(g_hWndDll, UDM_BULLET_SPLIT, TRUE, 0);
        EnableWindow(GetDlgItem(hwnd, IDZiDanFengSheng), false);
        EnableWindow(GetDlgItem(hwnd, IDNoZiDanFengSheng), true);
    }break;
    case IDNoZiDanFengSheng:
    {
        SendMessage(g_hWndDll, UDM_BULLET_SPLIT, FALSE, 0);
        EnableWindow(GetDlgItem(hwnd, IDNoZiDanFengSheng), false);
        EnableWindow(GetDlgItem(hwnd, IDZiDanFengSheng), true);
    }break;
    default:
        break;
    }
}
///////////////////////////////////////////////////////////////////////////////
INT_PTR WINAPI Dlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
    switch (uMsg) {
        lxy_HANDLE_DLGMSG(hwnd, WM_INITDIALOG, Dlg_OnInitDialog);//在此截获WM_INITDIALOG消息，进入Dlg初始化函数Dlg_OnInitDialog  
        lxy_HANDLE_DLGMSG(hwnd, WM_COMMAND, Dlg_OnCommand);////在此截获WM_COMMAND消息，在Dlg_OnCommand函数中统一处理
    }
    return FALSE;
}
///////////////////////////////////////////////////////////////////////////////
int WINAPI _tWinMain(HINSTANCE hinstExe, HINSTANCE, PTSTR pszCmdLine, int)
{

    HWND hwnd = FindWindow(TEXT("#32770")/*窗口类型*/, TEXT("Dialog")/*窗口名*/);
    /*寻找是否已经有一个窗口存在了*/

    if (IsWindow(hwnd)) {
        // 已经有一个窗口存在了 怎么处理 
        return 0;
    }
    else {
        try
        {
            LXY::RemoteThreadInjection rti(L"PlantsVsZombies.exe", "F:\\Code\\功能\\植物大战僵尸\\外挂程序\\Debug\\GongNengDll.dll");
            m_FindMyWindow();
        }
        catch (const LXY::m_HandleException&)
        {

        }
        DialogBoxParam(hinstExe, MAKEINTRESOURCE(IDD_DIALOGWinMain)/*对话框模板 一般是窗口资源ID 或者路径名 */,
            NULL/*拥有对话框的窗口的句柄*/, Dlg_Proc/*窗口处理函数*/, NULL/*传入给窗口处理函数的参数*/);
    }
    return 0;
}


