( function () { try {

  ///////////////////////////
  // Initialization
  
  const util = require("util");
  
  var dlg = window.gDlgObj = new Object();

  dlg.mTgtSceID = window.arguments[0];
  dlg.mTgtViewID = window.arguments[1];
  dd("CBPDlg> TargetScene="+dlg.mTgtSceID);

} catch (e) {debug.exception(e);} } )();

