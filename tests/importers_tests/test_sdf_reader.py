import pytest

import cuemol


def test_sdf_reader(test_data_path):
    svc = cuemol.getService("StreamManager")
    reader = svc.createHandler("sdf", 0)
    print(f"{reader=}")

    test_sdf_file = test_data_path / "test1.sdf"
    reader.setPath(str(test_sdf_file))
    obj = reader.createDefaultObj()
    reader.attach(obj)
    reader.read()
    reader.detach()
    natoms = obj.getAtomSize()
    print(f"{natoms=}")
    assert natoms == 37

    nbonds = obj.getBondSize()
    print(f"{nbonds=}")
    assert nbonds == 40


@pytest.fixture
def mol_from_sdf(test_data_path):
    svc = cuemol.getService("StreamManager")
    reader = svc.createHandler("sdf", 0)
    test_sdf_file = test_data_path / "test1.sdf"
    reader.setPath(str(test_sdf_file))
    obj = reader.createDefaultObj()
    reader.attach(obj)
    reader.read()
    reader.detach()
    return obj


def test_sdf_writer(mol_from_sdf, tmp_path):
    svc = cuemol.getService("StreamManager")
    writer = svc.createHandler("sdf", 1)
    print(f"{writer=}")

    out_file = tmp_path / "test.sdf"
    writer.setPath(str(out_file))
    writer.attach(mol_from_sdf)
    writer.write()
    writer.detach()

    with out_file.open() as f:
        result = f.readlines()
        # result = f.read()

    assert len(result) == 83
    assert result[3].rstrip() == " 37 40  0  0  0  0  0  0  0  0999 V2000"
    assert result[81].rstrip() == "M  END"
    assert result[82].rstrip() == "$$$$"


def test_sdf_chg(test_data_path, tmp_path):
    svc = cuemol.getService("StreamManager")
    reader = svc.createHandler("sdf", 0)
    print(f"{reader=}")

    test_sdf_file = test_data_path / "test_chg1.sdf"
    reader.setPath(str(test_sdf_file))
    obj = reader.createDefaultObj()
    reader.attach(obj)
    reader.read()
    reader.detach()

    ###

    writer = svc.createHandler("sdf", 1)
    print(f"{writer=}")

    out_file = tmp_path / "test.sdf"
    writer.setPath(str(out_file))
    writer.attach(obj)
    writer.write()
    writer.detach()

    with out_file.open() as f:
        result = f.readlines()

    assert len(result) == 128
    assert result[3].rstrip() == " 59 62  0  0  0  0  0  0  0  0999 V2000"
    assert result[125].rstrip() == "M  CHG  1  17   1"
    assert result[126].rstrip() == "M  END"
    assert result[127].rstrip() == "$$$$"
