'    WinFormsX - Windows GUI Framework for the FreeBASIC Compiler
'    Copyright (C) 2018 Paul Squires, PlanetSquires Software
'
'    This program is free software: you can redistribute it and/or modify
'    it under the terms of the GNU General Public License as published by
'    the Free Software Foundation, either version 3 of the License, or
'    (at your option) any later version.
'
'    This program is distributed in the hope that it will be useful,
'    but WITHOUT any WARRANTY; without even the implied warranty of
'    MERCHANTABILITY or FITNESS for A PARTICULAR PURPOSE.  See the
'    GNU General Public License for more details.


#include once "wfxApplication.bi"

''
''  wfxApplication Class
''

''  Main class for the entire wfxApplication. Everything starts and ends
''  through this class. 
''
''  https://msdn.microsoft.com/en-us/library/system.windows.forms.clsApplication(v=vs.110).aspx
''

Constructor wfxApplication()
   'print "app constructor"
end constructor

Destructor wfxApplication()
   ' Loop through the form collection and deallocate each form
'   dim pForm as wfxControl ptr
'   for i as long = 0 to this.Forms.Count - 1
'      pForm = this.Forms.ItemAt(i)
'      this.Forms.Remove(pForm)
'   NEXT
end destructor

''
''  Company name as stored in the program's resource file
''
Property wfxApplication.CompanyName() As CWSTR
   'TODO: CompanyName
   dim wszText as CWSTR = "PlanetSquires"
   return wszText
END Property

''
''  Product name as stored in the program's resource file
''
Property wfxApplication.ProductName() As CWSTR
   'TODO: ProductName
   dim wszText as CWSTR = "My App"
   return wszText
END Property

''
''  Product version as stored in the program's resource file
''
Property wfxApplication.ProductVersion() As CWSTR
   'TODO: ProductVersion
   dim wszText as CWSTR = "1.0.0"
   return wszText
END Property

''
''  Gets the path for the wfxApplication data that is shared amongst all users
''  (no trailing backspace)
''
Property wfxApplication.CommonAppDataPath() As CWSTR
   dim wszText as CWSTR = AfxGetSpecialFolderLocation(CSIDL_APPDATA)
   return wszText
END Property

''
''  Path for the wfxApplication data of the user (ie. AppData\Local)
''  (no trailing backspace)
''
Property wfxApplication.UserAppDataPath() As CWSTR
   dim wszText as CWSTR = AfxGetSpecialFolderLocation(CSIDL_LOCAL_APPDATA)
   return wszText
END Property

''
''  Full path and exe name for this running wfxApplication
''
Property wfxApplication.ExecutablePath() As CWSTR
   dim wszText as CWSTR = AfxGetExeFullPath()
   return wszText
END Property

''
''  Gets the path for the executable file that started the wfxApplication, 
''  not including the executable name. (no trailing backspace)
''
Property wfxApplication.StartupPath() As CWSTR
   dim wszText as CWSTR = rtrim(AfxGetExePathName, any "\/")
   return wszText
END Property

''
''  Processes all Windows messages currently in the message queue.
''
Function wfxApplication.DoEvents() As LRESULT
   AfxDoEvents()
   return 0
end function

''
''  Begins running a standard wfxApplication message loop without a form.
''
Function wfxApplication.Run() As LRESULT
' TODO: wfxApplication Run
   sleep
   return 0
end function

''
''  Begins running a standard wfxApplication message loop and 
''  makes the specified form visible.
''
Function wfxApplication.Run( Byref pForm As wfxForm ) As LRESULT

   Dim as MSG uMsg 

   ' Show the window and update its client area (this will also create any child controls)
   pForm.IsMainForm = true
   pForm.Show()
     
   ' Ensure the window is placed on screen should the user had changed 
   ' the logical ordering of a multiple display setup.
   AfxForceVisibleDisplay(pForm.pWindow->hWindow)
   
   ' Message loop
   Do While GetMessage(@uMsg, Null, 0, 0)
      ' Processes accelerator keys for menu commands
      If (pForm.pWindow->AccelHandle = 0) Or _
         (TranslateAccelerator(pForm.pWindow->hWindow, pForm.pWindow->AccelHandle, @uMsg)) = 0 Then
         If IsDialogMessage(pForm.pWindow->hWindow, @uMsg) = 0 Then
            TranslateMessage @uMsg    ' Translates virtual-key messages into character messages.
            DispatchMessage  @uMsg    ' Dispatches a message to a window procedure.
         End If
      End If
   Loop
   Function = uMsg.wParam

print "Message loop exited. Program ending."

end function


''    
''  Generic function to retrieve control object via Windows handle
''
function wfxApplication.GetControlByHandle( byval hWindow as hwnd ) as wfxLListNode ptr
   dim pNodeCtrl as wfxLListNode ptr
   dim pNode as wfxLListNode ptr
   dim pForm as wfxForm ptr
   
   ' Search all Forms and each control in each Form.
   pNode = this.Forms.get_first
   do until pNode = 0
      ' Does the hWindow match the form itself
      if pNode->hWindow = hWindow then return pNode
      ' Search all the controls on the form for a match
      pForm = cast(wfxForm ptr, pNode->pData)
      pNodeCtrl = pForm->Controls.search_handle(hWindow)
      if pNodeCtrl then return pNodeCtrl
      ' Get the form in the application   
      pNode = this.Forms.get_next(pNode)
   LOOP
   function = 0
end function

''
''  Internal function to set the elements of the various EventArg types.
''
Function wfxApplication.SetEventArgs( byval uMsg   as UINT, _
                                      ByVal wParam As WPARAM, _
                                      ByVal lParam As LPARAM, _
                                      byref e      as wfxEventArgs _ 
                                      ) as Long
   e.Message = uMsg
   e.wParam  = wParam
   e.lParam  = lParam
   e.Handled = false
   e.Cancel  = false    ' cancel a form close
   e.Ctrl    = (GetKeyState(VK_CONTROL) and &H8000)
   e.Shift   = (GetKeyState(VK_SHIFT) and &H8000)
   e.Alt     = (GetKeyState(VK_MENU) and &H8000)

   select case uMsg
      case WM_MOUSEMOVE, _
           WM_LBUTTONDOWN, WM_MBUTTONDOWN, WM_RBUTTONDOWN, _
           WM_LBUTTONUP, WM_MBUTTONUP, WM_RBUTTONUP, _
           WM_LBUTTONDBLCLK, WM_MBUTTONDBLCLK, WM_RBUTTONDBLCLK
         e.x = GET_X_LPARAM(lParam)
         e.y = GET_Y_LPARAM(lParam)
         e.LButton = (wParam and MK_LBUTTON)
         e.MButton = (wParam and MK_MBUTTON)
         e.RButton = (wParam and MK_RBUTTON)

      case WM_KEYDOWN, WM_KEYUP
         e.KeyCode = wParam

      case WM_CHAR   
         e.KeyChar = wParam
   END SELECT
         
   function = 0
END FUNCTION
 
''    
''  wfxApplication General Window procedure
''
static Function wfxApplication.WndProc( ByVal hWin   As HWnd, _
                                        ByVal uMsg   As UINT, _
                                        ByVal wParam As WPARAM, _
                                        ByVal lParam As LPARAM _
                                        ) As LRESULT


   dim pNode as wfxLListNode ptr
   dim e     as wfxEventArgs 
   
   dim pCtrl      as wfxControl ptr
   dim pForm      as wfxForm ptr
   dim pLabel     as wfxLabel ptr
   dim pButton    as wfxButton ptr
   dim pTextBox   as wfxTextBox ptr
   dim pCheckBox  as wfxCheckBox ptr
   dim pListBox   as wfxListBox ptr
   dim pStatusBar as wfxStatusBar ptr
   
   pNode = Application.GetControlByHandle(hWin)
   if pNode then 
      pCtrl = cast(wfxControl ptr, pNode->pData)
      select case pNode->CtrlType 
         CASE ControlType.Form:       pForm  = cast(wfxForm ptr, pNode->pData)
         CASE ControlType.Label:      pLabel = cast(wfxLabel ptr, pNode->pData)
         CASE ControlType.Button:     pButton = cast(wfxButton ptr, pNode->pData)
         CASE ControlType.TextBox:    pTextBox = cast(wfxTextBox ptr, pNode->pData)
         CASE ControlType.CheckBox:   pCheckBox = cast(wfxCheckBox ptr, pNode->pData)
         CASE ControlType.ListBox:    pListBox = cast(wfxListBox ptr, pNode->pData)
         CASE ControlType.StatusBar:  pStatusBar = cast(wfxStatusBar ptr, pNode->pData)
      END SELECT
   end if   

   Application.SetEventArgs(uMsg, wParam, lParam, e)
    
   ' Send to AllEvents handler should it be active
   if pForm andalso pForm->OnAllEvents then pForm->OnAllEvents(*pForm, e)
   if pLabel andalso pLabel->OnAllEvents then pLabel->OnAllEvents(*pLabel, e)
   if pButton andalso pButton->OnAllEvents then pButton->OnAllEvents(*pButton, e)
   if pTextBox andalso pTextBox->OnAllEvents then pTextBox->OnAllEvents(*pTextBox, e)
   if pCheckBox andalso pCheckBox->OnAllEvents then pCheckBox->OnAllEvents(*pCheckBox, e)
   if pListBox andalso pListBox->OnAllEvents then pListBox->OnAllEvents(*pListBox, e)
   if pStatusBar andalso pStatusBar->OnAllEvents then pStatusBar->OnAllEvents(*pStatusBar, e)

   ' If the event was handled in the OnAllEvents then return true
   if e.Handled THEN return true


   ' Attempt to process the message using any defined functions
   Select Case uMsg
      case WM_ACTIVATE
         if pForm then 
            ' Get the activation state
            if LoWord(wParam) = WA_INACTIVE THEN  ' Deactivated
               if pForm->OnDeactivate THEN pForm->OnDeactivate(*pForm, e)
            else ' Activated   
               if pForm->OnActivated THEN pForm->OnActivated(*pForm, e)
            END IF
         END IF

      case WM_CLOSE
         if pForm then 
            if pForm->OnFormClosing then 
               pForm->OnFormClosing(*pForm, e)
               if e.Cancel THEN return true
            end if   
            if pForm->IsModal then 
               ' Enables parent window keeping parent's zorder
               EnableWindow( GetParent(hWin), true)
               DestroyWindow(hWin)
            end if    
         end if   
          
      Case WM_DESTROY
         if pForm andalso pForm->OnFormClosed then pForm->OnFormClosed(*pForm, e)
         if pLabel andalso pLabel->OnDestroy then pLabel->OnDestroy(*pLabel, e)
         if pButton andalso pButton->OnDestroy then pButton->OnDestroy(*pButton, e)
         if pTextBox andalso pTextBox->OnDestroy then pTextBox->OnDestroy(*pTextBox, e)
         if pCheckBox andalso pCheckBox->OnDestroy then pCheckBox->OnDestroy(*pCheckBox, e)
         if pListBox andalso pListBox->OnDestroy then pListBox->OnDestroy(*pListBox, e)
         if pStatusBar andalso pStatusBar->OnDestroy then pStatusBar->OnDestroy(*pStatusBar, e)
         ' If this is the main form then post a quit message
         if pForm then 
            if pForm->IsMainForm THEN 
               PostQuitMessage(0)
            end if
         else
            ' This was a subclassed control so return to the subclass procedure
            ' and remove the subclass because now the final Destroy event handlers
            ' have been processed.
            return true      
         END IF
  
      case WM_COMMAND
         select CASE hiword(wParam)
            case BN_CLICKED
               ' wParam: LOWORD contains the button's control identifier. 
               '         HIWORD specifies the notification code.
               ' lParam: Handle to the button.
               ' Determine the pointer based on the Window handle generating the notification.
               pNode = Application.GetControlByHandle( cast(hwnd, lParam) )
               if pNode then 
                  pCtrl = cast(wfxControl ptr, pNode->pData)
                  select case pNode->CtrlType 
                     CASE ControlType.Label
                        pLabel = cast(wfxLabel ptr, pNode->pData)
                        if pLabel andalso pLabel->OnClick then pLabel->OnClick(*pLabel, e)
                     CASE ControlType.Button
                        pButton = cast(wfxButton ptr, pNode->pData)
                        if pButton andalso pButton->OnClick then pButton->OnClick(*pButton, e)
                     CASE ControlType.CheckBox
                        pCheckBox = cast(wfxCheckBox ptr, pNode->pData)
                        if pCheckBox andalso pCheckBox->OnClick then pCheckBox->OnClick(*pCheckBox, e)
                  END SELECT
               end if

            case LBN_DBLCLK
               ' wParam: LOWORD contains the button's control identifier. 
               '         HIWORD specifies the notification code.
               ' lParam: Handle to the button.
               ' Determine the pointer based on the Window handle generating the notification.
               pNode = Application.GetControlByHandle( cast(hwnd, lParam) )
               if pNode then 
                  pListBox = cast(wfxListBox ptr, pNode->pData)
                  if pListBox andalso pListBox->OnMouseDoubleClick then pListBox->OnMouseDoubleClick(*pListBox, e)
               end if
                     
            case LBN_SELCHANGE
               ' wParam: LOWORD contains the button's control identifier. 
               '         HIWORD specifies the notification code.
               ' lParam: Handle to the button.
               ' Determine the pointer based on the Window handle generating the notification.
               pNode = Application.GetControlByHandle( cast(hwnd, lParam) )
               if pNode then 
                  pListBox = cast(wfxListBox ptr, pNode->pData)
                  if pListBox andalso pListBox->OnClick then pListBox->OnClick(*pListBox, e)
               end if
         END SELECT
 
      case WM_NOTIFY
         dim pNMHDR as NMHDR ptr = cast(NMHDR ptr, lParam)
         Select Case pNMHDR->code 
            Case NM_CLICK
               ' Process clicking on the panels in the statusbar
               if pForm then
                  pNode = pForm->Controls.search_controltype(ControlType.StatusBar)
                  if pNode then 
                     pStatusBar = cast(wfxStatusBar ptr, pNode->pData)
                     if pStatusBar then
                        Dim lpnm As NMMOUSE Ptr = Cast(NMMOUSE Ptr, pNMHDR)
                        e.PanelClickIndex = lpnm->dwItemSpec  
                        e.x = lpnm->pt.x: e.y = lpnm->pt.y
                        if pStatusBar andalso pStatusBar->OnClick then pStatusBar->OnClick(*pStatusBar, e)
                     end if
                  end if
               end if
         end select
               
      Case WM_SIZE
         if pForm then 
            if pForm->OnResize then pForm->OnResize(*pForm, e)
            ' If a StatusBar exists on this form then resize it.
            pNode = pForm->Controls.search_controltype(ControlType.StatusBar)
            if pNode then
               pStatusBar = cast(wfxStatusBar ptr, pNode->pData)
               SendMessage(pStatusBar->hWindow, uMsg, wParam, lParam)
            END IF
         END IF
      
      case WM_CTLCOLORSTATIC, WM_CTLCOLOREDIT 
         ' wParam: HDC of the control. 
         ' lParam: Handle to the control.
         ' Need to determine the child control handle that is sending the request to be painted.
         pNode = Application.GetControlByHandle( cast(hwnd, lParam) )
         if pNode then 
            pCtrl = cast(wfxControl ptr, pNode->pData)
            if pCtrl THEN 
               SetTextColor(cast(HDC, wParam), pCtrl->ForeColor)
               SetBkColor(cast(HDC, wParam), pCtrl->BackColor)
               Return Cast(LRESULT, pCtrl->hBackBrush)
            end if   
         end if
         
      case WM_ERASEBKGND
         if pForm then 
            dim as Rect rc
            GetClientRect(hWin, @rc)
            FillRect(Cast(HDC, wParam), @rc, pForm->hBackBrush)
            return true
         end if   
          
      case WM_MOUSEMOVE
         if pForm andalso pForm->OnMouseMove then pForm->OnMouseMove(*pForm, e)
         if pLabel andalso pLabel->OnMouseMove then pLabel->OnMouseMove(*pLabel, e)
         if pButton andalso pButton->OnMouseMove then pButton->OnMouseMove(*pButton, e)
         if pTextBox andalso pTextBox->OnMouseMove then pTextBox->OnMouseMove(*pTextBox, e)
         if pCheckBox andalso pCheckBox->OnMouseMove then pCheckBox->OnMouseMove(*pCheckBox, e)
         if pListBox andalso pListBox->OnMouseMove then pListBox->OnMouseMove(*pListBox, e)
         if pStatusBar andalso pStatusBar->OnMouseMove then pStatusBar->OnMouseMove(*pStatusBar, e)
         ' There is no such thing as a WM_MOUSEENTER message so we need to simulate it
         ' here by using a tracking variable and TRACKMOUSEEVENT.
         if pCtrl then
            if pCtrl->IsTracking = false then
               pCtrl->IsTracking = true
               dim tme as TrackMouseEvent
               tme.cbSize = sizeof(TrackMouseEvent)
               tme.dwFlags = TME_HOVER or TME_LEAVE
               tme.hwndTrack = hWin
               TrackMouseEvent(@tme) 
               if pForm andalso pForm->OnMouseEnter then pForm->OnMouseEnter(*pForm, e)
               if pLabel andalso pLabel->OnMouseEnter then pLabel->OnMouseEnter(*pLabel, e)
               if pButton andalso pButton->OnMouseEnter then pButton->OnMouseEnter(*pButton, e)
               if pTextBox andalso pTextBox->OnMouseEnter then pTextBox->OnMouseEnter(*pTextBox, e)
               if pCheckBox andalso pCheckBox->OnMouseEnter then pCheckBox->OnMouseEnter(*pCheckBox, e)
               if pListBox andalso pListBox->OnMouseEnter then pListBox->OnMouseEnter(*pListBox, e)
               if pStatusBar andalso pStatusBar->OnMouseEnter then pStatusBar->OnMouseEnter(*pStatusBar, e)
            END IF
         END IF
          
      case WM_MOUSELEAVE
         if pCtrl then pCtrl->IsTracking = false 
         if pForm andalso pForm->OnMouseLeave then pForm->OnMouseLeave(*pForm, e)
         if pLabel andalso pLabel->OnMouseLeave then pLabel->OnMouseLeave(*pLabel, e)
         if pButton andalso pButton->OnMouseLeave then pButton->OnMouseLeave(*pButton, e)
         if pTextBox andalso pTextBox->OnMouseLeave then pTextBox->OnMouseLeave(*pTextBox, e)
         if pCheckBox andalso pCheckBox->OnMouseLeave then pCheckBox->OnMouseLeave(*pCheckBox, e)
         if pListBox andalso pListBox->OnMouseLeave then pListBox->OnMouseLeave(*pListBox, e)
         if pStatusBar andalso pStatusBar->OnMouseLeave then pStatusBar->OnMouseLeave(*pStatusBar, e)
      
      case WM_MOUSEHOVER
         if pForm andalso pForm->OnMouseHover then pForm->OnMouseHover(*pForm, e)
         if pLabel andalso pLabel->OnMouseHover then pLabel->OnMouseHover(*pLabel, e)
         if pButton andalso pButton->OnMouseHover then pButton->OnMouseHover(*pButton, e)
         if pTextBox andalso pTextBox->OnMouseHover then pTextBox->OnMouseHover(*pTextBox, e)
         if pCheckBox andalso pCheckBox->OnMouseHover then pCheckBox->OnMouseHover(*pCheckBox, e)
         if pListBox andalso pListBox->OnMouseHover then pListBox->OnMouseHover(*pListBox, e)
         if pStatusBar andalso pStatusBar->OnMouseHover then pStatusBar->OnMouseHover(*pStatusBar, e)
            
      case WM_LBUTTONDBLCLK, WM_MBUTTONDBLCLK, WM_RBUTTONDBLCLK
         if pForm andalso pForm->OnMouseDoubleClick then pForm->OnMouseDoubleClick(*pForm, e)
         if pLabel andalso pLabel->OnMouseDoubleClick then pLabel->OnMouseDoubleClick(*pLabel, e)
         if pButton andalso pButton->OnMouseDoubleClick then pButton->OnMouseDoubleClick(*pButton, e)
         if pTextBox andalso pTextBox->OnMouseDoubleClick then pTextBox->OnMouseDoubleClick(*pTextBox, e)
         if pCheckBox andalso pCheckBox->OnMouseDoubleClick then pCheckBox->OnMouseDoubleClick(*pCheckBox, e)
         if pStatusBar andalso pStatusBar->OnMouseDoubleClick then pStatusBar->OnMouseDoubleClick(*pStatusBar, e)
         ' For Listboxes, the doubleclick is handled through the LBN_DBLCLK notification.
         
      case WM_LBUTTONDOWN, WM_MBUTTONDOWN, WM_RBUTTONDOWN
         if pForm andalso pForm->OnMouseDown then pForm->OnMouseDown(*pForm, e)
         if pLabel andalso pLabel->OnMouseDown then pLabel->OnMouseDown(*pLabel, e)
         if pButton andalso pButton->OnMouseDown then pButton->OnMouseDown(*pButton, e)
         if pTextBox andalso pTextBox->OnMouseDown then pTextBox->OnMouseDown(*pTextBox, e)
         if pCheckBox andalso pCheckBox->OnMouseDown then pCheckBox->OnMouseDown(*pCheckBox, e)
         if pListBox andalso pListBox->OnMouseDown then pListBox->OnMouseDown(*pListBox, e)
         if pStatusBar andalso pStatusBar->OnMouseDown then pStatusBar->OnMouseDown(*pStatusBar, e)

      case WM_LBUTTONUP, WM_MBUTTONUP, WM_RBUTTONUP
         if pForm andalso pForm->OnMouseUp then pForm->OnMouseUp(*pForm, e)
         if pLabel andalso pLabel->OnMouseUp then pLabel->OnMouseUp(*pLabel, e)
         if pButton andalso pButton->OnMouseUp then pButton->OnMouseUp(*pButton, e)
         if pTextBox andalso pTextBox->OnMouseUp then pTextBox->OnMouseUp(*pTextBox, e)
         if pCheckBox andalso pCheckBox->OnMouseUp then pCheckBox->OnMouseUp(*pCheckBox, e)
         if pListBox andalso pListBox->OnMouseUp then pListBox->OnMouseUp(*pListBox, e)
         if pStatusBar andalso pStatusBar->OnMouseUp then pStatusBar->OnMouseUp(*pStatusBar, e)
         ' Forms convert LBUTTONUP to simulate OnClick()
         if uMsg = WM_LBUTTONUP then
            if pForm andalso pForm->OnClick then pForm->OnClick(*pForm, e)
            if pTextBox andalso pTextBox->OnClick then pTextBox->OnClick(*pTextBox, e)
         end if
         
      case WM_SETFOCUS
         if pButton andalso pButton->OnGotFocus then pButton->OnGotFocus(*pButton, e)
         if pTextBox andalso pTextBox->OnGotFocus then pTextBox->OnGotFocus(*pTextBox, e)
         if pCheckBox andalso pCheckBox->OnGotFocus then pCheckBox->OnGotFocus(*pCheckBox, e)
         if pListBox andalso pListBox->OnGotFocus then pListBox->OnGotFocus(*pListBox, e)

      case WM_KILLFOCUS
         if pButton andalso pButton->OnLostFocus then pButton->OnLostFocus(*pButton, e)
         if pTextBox andalso pTextBox->OnLostFocus then pTextBox->OnLostFocus(*pTextBox, e)
         if pCheckBox andalso pCheckBox->OnLostFocus then pCheckBox->OnLostFocus(*pCheckBox, e)
         if pListBox andalso pListBox->OnLostFocus then pListBox->OnLostFocus(*pListBox, e)

      case WM_KEYDOWN
         if pForm andalso pForm->OnKeyDown then pForm->OnKeyDown(*pForm, e)
         if pButton andalso pButton->OnKeyDown then pButton->OnKeyDown(*pButton, e)
         if pTextBox andalso pTextBox->OnKeyDown then pTextBox->OnKeyDown(*pTextBox, e)
         if pCheckBox andalso pCheckBox->OnKeyDown then pCheckBox->OnKeyDown(*pCheckBox, e)
         if pListBox andalso pListBox->OnKeyDown then pListBox->OnKeyDown(*pListBox, e)

      case WM_KEYUP
         if pForm andalso pForm->OnKeyUp then pForm->OnKeyUp(*pForm, e)
         if pButton andalso pButton->OnKeyUp then pButton->OnKeyUp(*pButton, e)
         if pTextBox andalso pTextBox->OnKeyUp then pTextBox->OnKeyUp(*pTextBox, e)
         if pCheckBox andalso pCheckBox->OnKeyUp then pCheckBox->OnKeyUp(*pCheckBox, e)
         if pListBox andalso pListBox->OnKeyUp then pListBox->OnKeyUp(*pListBox, e)

      case WM_CHAR
         if pForm andalso pForm->OnKeyPress then pForm->OnKeyPress(*pForm, e)
         if pButton andalso pButton->OnKeyPress then pButton->OnKeyPress(*pButton, e)
         if pTextBox andalso pTextBox->OnKeyPress then pTextBox->OnKeyPress(*pTextBox, e)
         if pCheckBox andalso pCheckBox->OnKeyPress then pCheckBox->OnKeyPress(*pCheckBox, e)
         if pListBox andalso pListBox->OnKeyPress then pListBox->OnKeyPress(*pListBox, e)
         
      Case WM_MOVE
         if pForm andalso pForm->OnMove then pForm->OnMove(*pForm, e)

   End Select
   
   ' For messages that we don't deal with make sure that Windows handles them.
   Function = DefWindowProc(hWin, uMsg, wParam, lParam)

End Function

'
'    
'  wfxApplication General Subclass Window procedure
'
static Function wfxApplication.SubclassProc ( _
                  ByVal hWin   As HWnd, _                 ' // Control window handle
                  ByVal uMsg   As UINT, _                 ' // Type of message
                  ByVal wParam As WPARAM, _               ' // First message parameter
                  ByVal lParam As LPARAM, _               ' // Second message parameter
                  ByVal uIdSubclass As UINT_PTR, _        ' // The subclass ID
                  ByVal dwRefData As DWORD_PTR _          ' // Pointer to reference data
                  ) As LRESULT

   Select Case uMsg
      Case WM_GETDLGCODE
         ' All keyboard input
         Function = DLGC_WANTALLKEYS

      case WM_MOUSEMOVE, _
           WM_LBUTTONDBLCLK, WM_MBUTTONDBLCLK, WM_RBUTTONDBLCLK, _
           WM_LBUTTONDOWN, WM_MBUTTONDOWN, WM_RBUTTONDOWN, _
           WM_LBUTTONUP, WM_MBUTTONUP, WM_RBUTTONUP, _
           WM_SETFOCUS, WM_KILLFOCUS, _
           WM_KEYDOWN, WM_KEYUP, WM_CHAR, _
           WM_MOUSELEAVE, WM_MOUSEHOVER
         wfxApplication.WndProc( hWin, uMsg, wParam, lParam )
         
      Case WM_DESTROY
         ' Handle any final Destroy messages for the control and then return
         ' here to remove the subclassing.
         wfxApplication.WndProc( hWin, uMsg, wParam, lParam )
         ' REQUIRED: Remove control subclassing
         RemoveWindowSubclass( hWin, @wfxApplication.SubclassProc, uIdSubclass )

      case else

   End Select
    
   ' For messages that we don't deal with
   Function = DefSubclassProc(hWin, uMsg, wParam, lParam)

End Function

