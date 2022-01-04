//#pragma comment(lib, "comctl32")

#include "cplusplus.h"
#include "comparator.h"

INT_PTR CALLBACK DlgProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE hInstance;
int threadCount=0;//запрет повторного запуска сканирования!

int WINAPI WinMain(HINSTANCE hlnst,HINSTANCE,LPSTR,int nCmdShow)
{
  hInstance=hlnst;
  InitCommonControls();
  try
  {
    DialogBox (hlnst, MAKEINTRESOURCE(IDD_DIALOG),NULL, DlgProc);
  }
  catch(const std::filesystem::filesystem_error& e)
  {
    MessageBox(NULL, TEXT("Error create dialog"), TEXT("Error"), MB_OK);
  }
  return 0;
}

INT_PTR CALLBACK DlgProc(HWND hwnd,UINT msg, WPARAM wParam,LPARAM lParam)
{
  static HWND hWndList = 0;

  //порядок сортировки
  static int statusName=0;
  static int statusPath=0;
  static int statusSize=0;
  static int statusDate=0;
  
  //смещение координат списка
  static int dx=0;
  static int dy=0;
  static int dw=20;
  static int dh=60;

  //длинна колонок
   //int cxname;//200;
   //int cxpath;//600;
  // int cxsize;//150;
   //int cxdate;//150;

  switch(msg)
  {
  case WM_SIZE:
    {
      RECT rect;
      GetClientRect(hwnd, &rect);
      SetWindowPos(hWndList, 0, 0, 0, rect.right, rect.bottom, SWP_NOZORDER);
      
      
      /*
      RECT rect;

      GetWindowRect(hwnd,&rect);
      */
      
      int width=rect.right-rect.left;
      int height=rect.bottom-rect.top;

      ListView_SetColumnWidth(hWndList,0,(width/10)*3);
      ListView_SetColumnWidth(hWndList,1,(width/10)*4);
      ListView_SetColumnWidth(hWndList,2,(width/10));
      ListView_SetColumnWidth(hWndList, 3, -2);
      /*

      ListView_SetColumnWidth(hWndList,0,(width/10)*3);
      ListView_SetColumnWidth(hWndList,1,(width/10)*4);
      ListView_SetColumnWidth(hWndList,2,(width/10));
      ListView_SetColumnWidth(hWndList,3,((width/10)+(width/10))-40);

      ShowWindow(hWndList, SW_HIDE);
      MoveWindow(hWndList,
        dx,dy,
        width-dw,height-dh,
        true);
      ShowWindow(hWndList, SW_SHOW);
      */
      

      //MessageBox(NULL, TEXT("RESIZE!"), TEXT("RESIZE"), MB_OK);
     //SetWindowPos(hWndList,0,0,0,width,height,SWP_DRAWFRAME );//SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE
    }
    return TRUE;

  case LVM_SETITEMCOUNT://обновление списка
    {
      ListView_SetItemCountEx(hWndList,directory.size(),LVSICF_NOSCROLL);
    }
    return TRUE;
    
    case LVM_REDRAWITEMS://обновление списка
      {
        ListView_RedrawItems(hWndList, 0, directory.size()-1);
        UpdateWindow(hWndList);
      }
    return TRUE;

  case WM_INITDIALOG:
    {
     
      RECT rect;
      GetClientRect(hwnd,&rect);

      int width=rect.right-rect.left;
      int height=rect.bottom-rect.top;

      hWndList= CreateWindowEx(/*двойная рамкаWS_EX_DLGMODALFRAMEWS_EX_STATICEDGE*/0,
        WC_LISTVIEW,
        NULL,
        WS_BORDER|WS_CHILD|WS_VISIBLE|LVS_OWNERDATA|LVS_REPORT,
        dx,dy,
        1,1,//width-dw,height-dh,
        hwnd,
        NULL,
        hInstance,
        NULL);

      //SetWindowPos(hWndList,0,0,0,width,height,SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);WS_BORDER

      ListView_SetExtendedListViewStyle(hWndList,
        LVS_EX_DOUBLEBUFFER|/*сначала вся картинка отрисовывается в невидимом теневом буфере (а не сразу на окне), а затем буфер выводится на окно (экран)*/ //не работает
        LVS_EX_FULLROWSELECT|/*выделение всей строки*/
        LVS_EX_GRIDLINES/*линия между ячейками*/
        );

      SetWindowPos(hWndList, 0, 0, 0, rect.right, rect.bottom, SWP_NOZORDER);
      /*
      ListView_SetColumnWidth(hWndList,0,(width/10)*3);
      ListView_SetColumnWidth(hWndList,1,(width/10)*4);
      ListView_SetColumnWidth(hWndList,2,(width/10));
      ListView_SetColumnWidth(hWndList, 3, -2);
      */
      //заголовок
      LV_COLUMN lvc;
      ZeroMemory(&lvc,sizeof(lvc));
      
      lvc.mask=LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
      lvc.fmt=LVCFMT_LEFT;//выравнивание
      
      lvc.cx=(width/10)*3;
      lvc.pszText=TEXT("File name");
      ListView_InsertColumn(hWndList, 0, &lvc);


      lvc.cx=(width/10)*4;
      lvc.pszText=TEXT("Folder Path");
      ListView_InsertColumn(hWndList, 1, &lvc);

      lvc.cx=(width/10);
      lvc.pszText=TEXT("File size");
      ListView_InsertColumn(hWndList, 2, &lvc);

      lvc.cx=-2;
      lvc.pszText=TEXT("Date/Time");
      ListView_InsertColumn(hWndList, 3, &lvc);
      ListView_SetColumnWidth(hWndList, 3, -2);

      ListView_SetItemCountEx(hWndList,directory.size(),LVSICF_NOSCROLL);
    }
    return TRUE;

  case WM_COMMAND:
    {
      switch(LOWORD(wParam))
      {
      case ID_STARTSCAN:
        {
          if(threadCount==0)
          {
            threadCount=1;//запрещаем повторный запуск
            std::filesystem::path p="C:\\windows";
            std::thread search(directory_traversal, p, hwnd);
            search.detach();
            MessageBox(NULL, TEXT("START..."), TEXT("scan"), MB_OK | MB_ICONINFORMATION);
          }
          else
          {
            MessageBox(NULL, TEXT("SCAN IS RUNNING"), TEXT("scan"), MB_OK | MB_ICONINFORMATION);
          }
        }
        break;
      case IDCANCEL:
        {
		  std::lock_guard<std::mutex> l(mutex_);
          EndDialog(hwnd,0);
        }
        break;
      }
    }
    return TRUE;
  
  case WM_NOTIFY:
    {
      switch (((LPNMHDR)lParam)->code)
      {
        /*
      case HDN_ITEMCHANGING:
        {
          ListView_SetColumnWidth(hWndList, 3, -2);
        }
        break;

        case HDN_ITEMCHANGED:
        {
          ListView_SetColumnWidth(hWndList, 3, -2);
        }
        break;
        
      case HDN_BEGINTRACK:
        {
          //SendMessage(hwnd,WM_SIZE,0,0);
            //ListView_SetColumnWidth(hWndList, 3, -2);            
        }
        
      case HDN_TRACK:
        {
          
        }
        
      case HDN_ENDTRACK://HDN_ENDTRACK://:
        {
          SendMessage(hWndList,WM_SIZE,0,0);
          //ListView_SetColumnWidth(hWndList, 3, -2);
        }
        break;
        //return TRUE;
        */
        case LVN_GETDISPINFO:
          {
            NMLVDISPINFO* plvdi = (NMLVDISPINFO*)lParam;
          switch (plvdi->item.iSubItem)//заполняем строку
          {
          case 0://"имя файла"
            {
              plvdi->item.pszText = (LPWSTR)directory[plvdi->item.iItem]._name.c_str();
              break;
            }

          case 1://"путь к файлу"
            {
              plvdi->item.pszText = (LPWSTR)directory[plvdi->item.iItem]._path.c_str();
              break;
            }
            case 2://"размер файла"
            {              
              plvdi->item.pszText = (LPWSTR)directory[plvdi->item.iItem]._filesize.c_str();
              break;
            }
            case 3://"дата"
            {
              plvdi->item.pszText = (LPWSTR)directory[plvdi->item.iItem]._creation.c_str();
              break;
            }
          default:
            return 0;
          }
        }
        break;
       
       case LVN_COLUMNCLICK:
        {
		   std::lock_guard<std::mutex> l(mutex_);
          LPNMLISTVIEW pnmv = (LPNMLISTVIEW) lParam;
          //NM_LISTVIEW *pnmv= (NM_LISTVIEW*)lParam;
          switch (pnmv->iSubItem)//клик по заголовку - сортировка
          {
          case 0://"имя файла"
            {
//               if(statusName==0)
//               {
//                 sort(directory.begin(), directory.end(), boost::bind(less<wstring>(), boost::bind(&data::_name, _1), boost::bind(&data::_name, _2)));//убывание
//                 statusName=1;
//               }
//               else
//               {
//                 sort(directory.begin(), directory.end(), boost::bind(not2(less_equal<wstring>()), boost::bind(&data::_name, _1), boost::bind(&data::_name, _2)));//возростание
//                 statusName=0;
//               }
            }//case 0:
            break;
          
          case 1://"путь"
            {
//               if(statusPath==0)
//               {
//                 sort(directory.begin(), directory.end(), boost::bind(less<wstring>(), boost::bind(&data::_path, _1), boost::bind(&data::_path, _2)));//убывание
//                 statusPath=1;
//               }
//               else
//               {
//                 sort(directory.begin(), directory.end(), boost::bind(not2(less_equal<wstring>()), boost::bind(&data::_path, _1), boost::bind(&data::_path, _2)));//возростание
//                 statusPath=0;
//               }
            }//case 1:
            break;

            case 2://"размер - это важно:)"
            {
//               if(statusSize==0)
//               {
//                 sort(directory.begin(), directory.end(), boost::bind(less<uintmax_t>(), boost::bind(&data::_fsize, _1), boost::bind(&data::_fsize, _2)));//убывание
//                 statusSize=1;
//               }
//               else
//               {
//                 sort(directory.begin(), directory.end(), boost::bind(not2(less_equal<uintmax_t>()), boost::bind(&data::_fsize, _1), boost::bind(&data::_fsize, _2)));//возростание
//                 statusSize=0;
//               }
            }//case 2:
            break;
            case 3://"дата"
            {
//               if(statusDate==0)
//               {
//                 sort(directory.begin(), directory.end(), compareTimeLess);//убывание
//                 statusDate=1;
//               }
//               else
//               {
//                 sort(directory.begin(), directory.end(), compareTimeNotLess);//возростание
//                 statusDate=0;
//               }
            }//case 3:
            break;
          }//switch (pnmv->iSubItem)
          SendMessage(hwnd,LVM_SETITEMCOUNT,0,0);
          SendMessage(hwnd,LVM_REDRAWITEMS,0,0);
          //SendMessage(hwnd,ID_REFRESH,0,0);
        }//LVN_COLUMNCLICK
        break;
      }//switch (((LPNMHDR)lParam)->code)
    }//WM_NOTIFY
  default:
    return FALSE;
  }//switch(msg)
}//DlgProc