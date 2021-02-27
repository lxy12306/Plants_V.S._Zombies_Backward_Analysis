#include "F:\Code\object\WinUserHandle\MyWindows.h"
#include <windowsx.h>
#include "resource.h"
#include <tchar.h>
#include "F:\Code\object\ProcessAndHwndAndThread\Inject\MyInject.h"
#include "Messages.h"

///////////////////////////////////////////////////////////////////////////////
//ȫ�ֱ���
HWND g_hWndDll = NULL; //������Ϣ�ľ��

///////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Dlg��ʼ����ʱ��ɵ�����
/// </summary>
/// <param name="hwnd">���ھ��</param>
/// <param name="hwndFocus">��ʼĬ��Focus�Ŀؼ����</param>
/// <param name="lParam">����</param>
/// <returns>�Ƿ��ʼ���ɹ�</returns>
BOOL Dlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) 
{

    return(TRUE);
}
//////////////////////////////////////////////////////////////////////////////
/// <summary>
/// ���ڿؼ��Ĵ�����
/// </summary>
/// <param name="hwnd">���ھ��</param>
/// <param name="id">�ؼ�ID</param>
/// <param name="hwndCtl">�ؼ����</param>
/// <param name="codeNotify">Ĭ��Ϊ0</param>
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
        DWORD dwSunLight = strtoul(atcBuffer, NULL, 10);//10����ת��
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
        lxy_HANDLE_DLGMSG(hwnd, WM_INITDIALOG, Dlg_OnInitDialog);//�ڴ˽ػ�WM_INITDIALOG��Ϣ������Dlg��ʼ������Dlg_OnInitDialog  
        lxy_HANDLE_DLGMSG(hwnd, WM_COMMAND, Dlg_OnCommand);////�ڴ˽ػ�WM_COMMAND��Ϣ����Dlg_OnCommand������ͳһ����
    }
    return FALSE;
}
///////////////////////////////////////////////////////////////////////////////
int WINAPI _tWinMain(HINSTANCE hinstExe, HINSTANCE, PTSTR pszCmdLine, int)
{

    HWND hwnd = FindWindow(TEXT("#32770")/*��������*/, TEXT("Dialog")/*������*/);
    /*Ѱ���Ƿ��Ѿ���һ�����ڴ�����*/

    if (IsWindow(hwnd)) {
        // �Ѿ���һ�����ڴ����� ��ô���� 
        return 0;
    }
    else {
        try
        {
            LXY::RemoteThreadInjection rti(L"PlantsVsZombies.exe", "F:\\Code\\����\\ֲ���ս��ʬ\\��ҳ���\\Debug\\GongNengDll.dll");
            m_FindMyWindow();
        }
        catch (const LXY::m_HandleException&)
        {

        }
        DialogBoxParam(hinstExe, MAKEINTRESOURCE(IDD_DIALOGWinMain)/*�Ի���ģ�� һ���Ǵ�����ԴID ����·���� */,
            NULL/*ӵ�жԻ���Ĵ��ڵľ��*/, Dlg_Proc/*���ڴ�����*/, NULL/*��������ڴ������Ĳ���*/);
    }
    return 0;
}


