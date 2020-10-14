import megaind as ind
import sys


def test_get4_20In(v = False):
    for i in range(1, 5):
        val = ind.get4_20In(0, i)
        if v:
            print('4 - 20mA input channel ' + str(i) + ' read ' + str(val))
        assert val >= 0, "Should be greater than or equal to 0" #disconnected channel
        assert val <= 20, "Should be less than or equal to 20"


def test_set4_20Out(v = False):
    for i in range(1, 5):
        ind.set4_20Out(0, i, 4)
        if v:
            print('4 - 20mA output channel '+ str(i) + ' set to 4mA')


def test_get4_20Out(v = False):
    for i in range(1, 5):
        ind.set4_20Out(0, i, 4)
        val = ind.get4_20Out(0, i)
        if v:
            print('4 - 20mA output channel ' + str(i) + ' read ' + str(val))
        assert val == 4, "Should be 4"


if __name__ == "__main__":
    verb = False
    if len(sys.argv) > 1:
        if sys.argv[1] == '-v':
            verb = True
    test_get4_20In(verb)
    print("get4_20In() passed")
    test_set4_20Out(verb)
    print("set4_20Out() passed")
    test_get4_20Out(verb)
    print("get4_20Out() passed")
