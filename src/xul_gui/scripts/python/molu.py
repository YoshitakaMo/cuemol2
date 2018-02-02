#
#  Molecule utility functions
#

import cuemol

# def forEachAtom(aMol, aSel, aFn):
#     uid = aMolObj.uid
#     aSelObj = cuemol.sel(aSel, uid)
# 
#     iter = cuemol.createObj("AtomIterator")
# 
#     iter.target = aMolObj;
#     iter.sel = aSelObj;
# 
#     for (iter.first(); iter.hasMore(); iter.next()) {
# 	let atom = iter.get();
# 	if (aFn(atom))
# 	    break;
#     }
# } 

class AtomIter:
    def __init__(self, aMol, aSel):
        molObj = cuemol.obj(aMol)
        uid = molObj.uid
        selObj = cuemol.sel(aSel, molObj.scene_uid)

        self.mIter = cuemol.createObj("AtomIterator");
        self.mIter.target = molObj;
        self.mIter.sel = selObj;

        self.mIter.first()

    def __iter__(self):
        return self

    def __next__(self):
        if not self.mIter.hasMore():
            raise StopIteration()
        rval = self.mIter.get()
        self.mIter.next()
        return rval

def forEachResid(aMol, aSel, aFn):
    aMolObj = cuemol.obj(aMol)

    uid = aMolObj.uid
    aSelObj = cuemol.sel(aSel, molObj.scene_uid)

    iter = cuemol.createObj("ResidIterator");
    #let sel = cuemol.createObj("SelCommand");
    #sel.compile(aSelStr, 0);

    iter.target = aMolObj;
    iter.sel = aSelObj;

    iter.first()
    while True:
        if not iter.hasMore():
            break
        resid = iter.get()
        if aFn(resid):
            break
        iter.next()

class ResidIter:
    def __init__(self, aMol, aSel):
        molObj = cuemol.obj(aMol)
        selObj = cuemol.sel(aSel, molObj.scene_uid)

        self.mIter = cuemol.createObj("ResidIterator");
        self.mIter.target = molObj;
        self.mIter.sel = selObj;

        self.mIter.first()

    def __iter__(self):
        return self

    def __next__(self):
        if not self.mIter.hasMore():
            raise StopIteration()
        rval = self.mIter.get()
        self.mIter.next()
        return rval

def sameAtom(aMol, aAtom):
    chname = aAtom.chainName
    resid = aAtom.residIndex
    aname = aAtom.name
    return aMol.getAtom(chname, resid, aname)

def showArrow(aMol, aRendName, aPos1, aPos2, aMol2=None):
    rend = aMol.getRendererByName(aRendName)

    if rend:
        if not rend.type_name=="atomintr":
            rend = aMol.createRenderer("atomintr")
    else:
        rend = aMol.createRenderer("atomintr")
        rend.name = aRendName

    if not rend:
        return

    rend.mode = "fancy"
    rend.captype_end = "arrow"
    rend.captype_start = "arrow"
    rend.width = 0.2
    rend.stipple0 = 0
    rend.stipple1 = 0

    if cuemol.isimpl(aPos1, 'Vector') and cuemol.isimpl(aPos2, 'Vector'):
#        pass
        rend.appendBy2Vecs(aPos1, aPos2)
    elif cuemol.isimpl(aPos1, 'MolAtom') and cuemol.isimpl(aPos2, 'MolAtom'):
        mol2=None
        if aMol2==None:
            mol2 = aMol
        else:
            mol2 = aMol2
#        pass
        rend.appendById(aPos1.id, mol2.uid, aPos2.id, False)
    else:
        raise RuntimeError("showArrow() unknown aPos1/aPos2 type")
