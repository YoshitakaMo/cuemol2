import pytest
import cuemol

def test_qlib_msglog():
    svc = cuemol.getService("MsgLog")
    assert svc

    svc.write("test_str")

    svc.writeln("test_str")
    
    s = svc.getAccumMsg()
    print(f"{s=}")

    
def test_qlib_msglog_redir(tmp_path):
    print(tmp_path)
    svc = cuemol.getService("MsgLog")
    log_file = tmp_path / "log.txt"
    svc.setFileRedirPath(str(log_file))
    svc.writeln("test_str")
    chk = svc.getFileRedirPath() 
    print(f"{chk=}")
    assert chk.endswith("log.txt")
    svc.setFileRedirPath("")
    assert False
