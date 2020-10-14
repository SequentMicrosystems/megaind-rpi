import megaind as ind
import sys


def test_getOptoCh(v):
    for i in range (1, 5):
        val = ind.getOptoCh(0, i)
        if v :
            print ('Opto cupled input channel ' + str(i) + " read value " + str(val))
        assert val >= 0, "Should be greater than or equal to 0"
        assert val <= 1, "Should be less than or equal to 1"


def test_getOpto(v):
    val = ind.getOpto(0)
    if v :
        print("Opto input read " + str(val))
    assert val >= 0, "Should be greater than or equal to 0"
    assert val <= 15, "Should be less than or equal to 15"


def test_getOptoCount(v):
    for i in range (1, 5):
        val = ind.getOptoCount(0, i)
        if v :
            print ('Opto cupled input contor channel ' + str(i) + " read value " + str(val))
        assert val >= 0, "Should be greater than or equal to 0"


def test_rstOptoCount(v):
    for i in range (1, 5):
        ind.rstOptoCount(0, i);
        val = ind.getOptoCount(0, i)
        if v :
            print ('Opto cupled input contor channel ' + str(i) + " read value " + str(val))
        assert val == 0, "Should be equal to 0"


def test_getOptoRisingCountEnable(v):
    for i in range (1, 5):
        val = ind.getOptoRisingCountEnable(0, i)
        if v :
            print ('Opto cupled input rising edge enable channel ' + str(i) + " read value " + str(val))
        assert val >= 0, "Should be greater than or equal to 0"


if __name__ == "__main__":
    verb = False
    if len(sys.argv) > 1:
        if sys.argv[1] == '-v':
            verb = True
    test_getOptoCh(verb)
    print("getOptoCh() passed")
    test_getOpto(verb)
    print("getOpto() passed")
    test_getOptoCount(verb)
    print ("getOptoCount() passed")
    test_rstOptoCount(verb)
    print ("rstOptoCount() passed")