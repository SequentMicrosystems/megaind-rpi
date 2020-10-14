import megaind as ind
import sys


def test_get0_10In(verb = False):
    for i in range(1, 5):
        val = ind.get0_10In(0, i)
        if verb :
            print('0 - 10V input channel '+ str(i) + ' read '+ str(val))
        assert val >= 0, "Should be greater than or equal to 0"
        assert val <= 10, "Should be less than or equal to 10"


def test_getpm10In(verb = False):
    for i in range(1, 5):
        val = ind.getpm10In(0, i)
        if verb :
            print('+/-10V input channel '+ str(i) + ' read '+ str(val))
        assert val >= -10


def test_set0_10Out(verb = False):
    for i in range(1, 5):
        ind.set0_10Out(0, i, 0)
        if verb :
            print('+/-10V output channel '+ str(i) + ' write 0.0')


def test_get0_10Out(verb = False):
    for i in range(1, 5):
        ind.set0_10Out(0, i, 0)
        val = ind.get0_10Out(0, i)
        if verb :
            print('0 - 10V output channel '+ str(i) + ' read '+ str(val))
        assert val == 0, "Should be 0"


if __name__ == "__main__":
    verb = False
    if len(sys.argv) > 1:
        if sys.argv[1] == '-v':
            verb = True
    test_get0_10In(verb)
    print("get0_10In() passed")
    test_getpm10In(verb)
    print("getpm10In() passed")
    test_set0_10Out(verb)
    print("set0_10Out() passed")
    test_get0_10Out(verb)
    print("get0_10Out() passed")