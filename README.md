# libdsm
  <li>This is a rewrote library based on the original libdsm library of Videolabs's organization.</li>
  <li>The original libdsm you can visit here: https://github.com/videolabs/libdsm.</li>
  <li>This library was developed to support iOS developers who want to handly implement this library to their projects instead of using static library that can not rewrite for personal purposes</li>
<h2>About</h2>
  lib Defective SMb (libDSM) is a SMB protocol client implementation in pure old C, with a lot less features than Samba but is simpler, and has with a more permissive license (currently LGPL + proprietary).
  The initial goal of this project is to have a library that can access most SMB shares to read files and that has with a license compatible with the iOS/Android/WinRT appstores in order to integrate it into VLC for iOS and VLC for Android.
  
<h2>Features</h2>
     Here's a list of the currently supported features:
     NETBIOS
      + Basic bi-directionnal NETBIOS name resolution
      + Hacky LAN SMB servers discovery (Listing all the smb servers on the LAN, no WINS, etc.)
      + Basic NETBIOS Session transport layer
  - SMB
      + Support only required parts of 'NT LM 0.12' (aka CIFS?) dialect.
      + User based authentication
      + List Shares
      + Browse folders
      + Read file
      + Write file
      + Delete file
      + Delete empty folder
      + Move and rename file/folder
      + Create new folder
      + No copy, lock, RPCs, etc. [Hum... yet]
     
<h2>Requirements</h2>
  libdsm works on iOS 7.0+ and is compatible with ARC projects.
