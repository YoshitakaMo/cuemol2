
Qm2Main.prototype.molClientTools = function()
{
  let stylestr = "chrome,modal,resizable=no,dependent,centerscreen";

  let data = Object();
  window.openDialog("chrome://cuemol2/content/tools/molclient-tools-dlg.xul",
                    "", stylestr, data);

  if (!data.ok) {
    dd("molclient tool dialog canceled.");
    return;
  }

  const mgr = cuemol.getService("MolClientManager");
  let con = mgr.connect(data.url);
  let obj = con.molFromSmiles(data.smiles);
  obj.name = data.molname;
  this.molClientTools_setupRend(obj, data);
}
  
Qm2Main.prototype.molClientTools_setupRend = function(obj, data)
{
  let scene_id = this.mMainWnd.getCurrentSceneID();

  // scene.addObject(obj);
  let obj_type = obj._wrapped.getClassName();
  let rend_types = obj.searchCompatibleRendererNames();
  
  let dlgdata = new Object();
  dlgdata.sceneID = scene_id;
  dlgdata.ok = false;
  dlgdata.bEditObjName = true;
  dlgdata.target = new Array();
  dlgdata.target[0] = new Object();
  dlgdata.target[0].name = obj.name;
  dlgdata.target[0].obj_type = obj_type;
  dlgdata.target[0].rend_types = rend_types;
  dlgdata.target[0].reader_name = "xxx";
  // dlgdata.target[0].reader = reader;
  dlgdata.target[0].preset_types = this.getCompatibleRendPresetNames(obj_type, scene_id);
  
  let result = this.doSetupRendDlg(dlgdata);
  if (!result.ok)
    return;

  let sc = cuemol.getScene(scene_id);
  result.new_obj = true;
  sc.startUndoTxn("Create new "+result.rendtype+" renderer");
  try {
    sc.addObject(obj);
    result.obj_id = obj.uid;
    rend = this.doSetupRend(sc, result);
  }
  catch (e) {
    dd("CreateRend> failed: "+e);
    debug.exception(e);
    sc.rollbackUndoTxn();
    util.alert(window, "ERROR: Create new representation; "+e);
    return;
  }
  sc.commitUndoTxn();
  
}
