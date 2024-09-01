( function () { try {

  ///////////////////////////
  // Initialization
  
  let dlg = window.gDlgObj = new Object();

  dlg.mData = window.arguments[0];
  dd("MolCliDlg> TargetData="+dlg.mData);
  dlg.mData.ok = false;

  dlg.onDialogAccept = function ()
  {
    const url = document.getElementById("mol-server-url").value;
    const smiles = document.getElementById("smiles-string").value;
    const molname = document.getElementById("mol-name").value;

    dd("url: " + url);
    dd("smiles: " + smiles);
    dd("molname: " + molname);

    this.mData.url = url;
    this.mData.smiles = smiles;
    this.mData.molname = molname;

    this.mData.ok = true;
  };

} catch (e) {debug.exception(e);} } )();

