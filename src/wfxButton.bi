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


Type wfxButton Extends wfxControl
   private:
      _TextAlign as ButtonAlignment = ButtonAlignment.MiddleCenter
    
   Public:
      Declare Property TextAlign() As ButtonAlignment
      Declare Property TextAlign( ByVal nValue As ButtonAlignment )
      Declare Constructor( byref wszName as wstring = "" )
      declare function Show(byval hWndParent as hwnd = 0) as long override

      OnAllEvents        as function( byref sender as wfxButton, byref e as EventArgs ) as LRESULT
      OnDestroy          as function( byref sender as wfxButton, byref e as EventArgs ) as LRESULT
      OnClick            as function( byref sender as wfxButton, byref e as EventArgs ) as LRESULT
      OnGotFocus         as function( byref sender as wfxButton, byref e as EventArgs ) as LRESULT
      OnLostFocus        as function( byref sender as wfxButton, byref e as EventArgs ) as LRESULT
      OnMouseMove        as function( byref sender as wfxButton, byref e as EventArgs ) as LRESULT
      OnMouseDown        as function( byref sender as wfxButton, byref e as EventArgs ) as LRESULT
      OnMouseUp          as function( byref sender as wfxButton, byref e as EventArgs ) as LRESULT
      OnMouseDoubleClick as function( byref sender as wfxButton, byref e as EventArgs ) as LRESULT
      OnMouseEnter       as function( byref sender as wfxButton, byref e as EventArgs ) as LRESULT
      OnMouseHover       as function( byref sender as wfxButton, byref e as EventArgs ) as LRESULT
      OnMouseLeave       as function( byref sender as wfxButton, byref e as EventArgs ) as LRESULT
      OnKeyDown          as function( byref sender as wfxButton, byref e as EventArgs ) as LRESULT
      OnKeyPress         as function( byref sender as wfxButton, byref e as EventArgs ) as LRESULT
      OnKeyUp            as function( byref sender as wfxButton, byref e as EventArgs ) as LRESULT
End Type

