import megaind as ind
import time


def flash_seq(n, delay=0.1, stack=0):
    for i in range(n):
        for j in range(4):
            ind.setLed(stack, j + 1, 1)
            time.sleep(delay)
        for j in range(4):
            ind.setLed(stack, j + 1, 0)
            time.sleep(delay)


def flash(n, delay=0.2, stack=0):
    for i in range(n):
        ind.setLedAll(stack, 15)
        time.sleep(delay)
        ind.setLedAll(stack, 0)
        time.sleep(delay)


if __name__ == "__main__":
    flash_seq(10)
    flash(10)
