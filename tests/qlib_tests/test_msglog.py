import pytest
import cuemol

def test_qlib_msglog():
    svc = cuemol.getService("MsgLog")
    assert svc

def test_qlib_msglog_write(capfd):
    svc = cuemol.getService("MsgLog")
    svc.write("test_str ")

    svc.writeln("test_str")
    
    captured = capfd.readouterr()
    print(f"XXX {captured.out=}")
    print(f"XXX {captured.err=}")
    assert "test_str test_str" in captured.out
    # assert False

def test_qlib_msglog_write(capfd):
    svc = cuemol.getService("MsgLog")
    svc.removeAccumMsg()
    svc.writeln("XXXXXXXXXX")
    s = svc.getAccumMsg()

    # msg should not be accumlated
    assert s == ""
    
def test_qlib_msglog_redir(tmp_path):
    print(tmp_path)
    svc = cuemol.getService("MsgLog")
    log_file = tmp_path / "log.txt"
    svc.setFileRedirPath(str(log_file))
    svc.writeln("YYYYYYYYYY")
    chk = svc.getFileRedirPath() 
    print(f"{chk=}")
    assert chk.endswith("log.txt")
    svc.setFileRedirPath("")
    svc.writeln("XXXXXXXXXX")

    with log_file.open() as f:
        dat = f.read()
    assert "YYYYYYYYYY" in dat
    assert "XXXXXXXXXX" not in dat
    # print(dat)
