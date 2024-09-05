import pytest
import cuemol

@pytest.fixture
def tv_obj():
    return cuemol.createObj("TimeValue")

def test_qlib_timeval(tv_obj):
    print(f"{tv_obj=}")
    print(f"{tv_obj.strval=}")
    assert tv_obj
    assert tv_obj.strval == "0"
    # assert False

def test_qlib_timeval_tostring(tv_obj):
    tv_obj.millisec = 123
    assert tv_obj.millisec == pytest.approx(123)
    assert tv_obj.toString() == "0.123"
    assert tv_obj.strval == "0.123"

    tv_obj.millisec = 123.456
    assert tv_obj.millisec == pytest.approx(123.456)
    assert tv_obj.toString() == "0.123"

    tv_obj.second = 123
    assert tv_obj.second == pytest.approx(123.0)
    assert tv_obj.toString() == "2:3"
    
    tv_obj.second = 123.456
    assert tv_obj.second == pytest.approx(123.456)
    assert tv_obj.toString() == "2:3.456"

    tv_obj.second = 123456.789
    assert tv_obj.second == pytest.approx(123456.789)
    assert tv_obj.toString() == "34:17:36.789"


def test_qlib_timeval_fromstring(tv_obj):
    tv_obj.strval = "0.123"
    assert tv_obj.millisec == pytest.approx(123)

    tv_obj.strval = "2:3"
    assert tv_obj.second == pytest.approx(123.0)

    tv_obj.strval = "2:3.456"
    assert tv_obj.second == pytest.approx(123.456)

    tv_obj.strval = "34:17:36.789"
    assert tv_obj.second == pytest.approx(123456.789)


def test_qlib_timeval_getxx(tv_obj):
    tv_obj.second = 123456.789
    assert tv_obj.getHour() == 34
    assert tv_obj.getMinute(True) == 17
    assert tv_obj.getSecond(True) == 36
    assert tv_obj.getMilliSec(True) == 789 

    assert tv_obj.getMinute(False) == 2057
    assert tv_obj.getSecond(False) == 123456
    assert tv_obj.getMilliSec(False) == 123456789 
   
def test_qlib_timeval_equals():
    tv1 = cuemol.createObj("TimeValue", "12:34:56.78")
    tv2 = cuemol.createObj("TimeValue", "12:34:56.78")
    assert tv1.equals(tv2)

    tv3 = cuemol.createObj("TimeValue", "34:56.78")
    assert not tv1.equals(tv3)
