import pytest
import cuemol


@pytest.fixture
def svc():
    return cuemol.getService("ProcessManager")

def test_qlib_procmgr(svc):
    print(svc.queue_len)
    print(svc.errormsg)
    assert svc.queue_len == 0
    assert svc.errormsg == ""
    
def test_qlib_procmgr_slotsize(svc):
    svc.setSlotSize(10)
    assert svc.getSlotSize() == 10

def test_qlib_procmgr_queue_task(svc):
    svc.queueTask("ls", "-la", "")

def test_qlib_procmgr_queue_task2(svc):
    result = svc.queueTask("sleep", "1", "")
    print(f"{result=}")
    is_alive = svc.isAlive(result)
    assert is_alive
    print(f"{is_alive=}")
    status = svc.getTaskStatus(result)
    print(f"{status=}")
    # 1: PM_RUNNING
    assert status == 1

    svc.waitForExit(result)
    status = svc.getTaskStatus(result)
    print(f"{status=}")
    # 1: PM_ENDED
    assert status == 2

    json = svc.doneTaskListJSON()
    print(f"{json=}")

    
def test_qlib_procmgr_queue_task3(svc):
    svc.setSlotSize(1)

    result = svc.queueTask("sleep", "1", "")
    result2 = svc.queueTask("sleep", "1", "")
    assert result != result2
    print(f"{result=}")
    is_alive = svc.isAlive(result)
    assert is_alive
    print(f"{is_alive=}")
    status = svc.getTaskStatus(result)
    print(f"{status=}")
    # 1: PM_RUNNING
    assert status == 1

    print(f"{svc.queue_len=}")
    assert svc.queue_len > 0

    # kill test
    svc.kill(result)
    status = svc.getTaskStatus(result)
    print(f"task1 killed, {status=}")
    # 1: PM_ENDED
    assert status == 2

    # waitfor second task
    svc.waitForExit(result2)
    status = svc.getTaskStatus(result2)
    print(f"task2 finished, {status=}")
    # 1: PM_ENDED
    assert status == 2

    json = svc.doneTaskListJSON()
    print(f"{json=}")

def test_qlib_procmgr_queue_task(svc, tmp_path):
    log_file = tmp_path / "log.txt"
    svc.setLogPath(str(log_file))
    result = svc.queueTask("ls", "-la", "")
    print(f"{log_file=}")

    svc.waitForExit(result)
    status = svc.getTaskStatus(result)
    
    # check log output
    outstr = svc.getResultOutput(result)
    print(f"{outstr=}")
    assert len(outstr) > 0

    # check file output
    with log_file.open() as f:
        log_cont = f.readlines()
    print(f"{log_cont=}")
    assert len(log_cont) > 0
