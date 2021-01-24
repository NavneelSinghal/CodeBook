# fast IO
import sys
input = sys.stdin.readline
def print(x, end='\n'):
    sys.stdout.write(str(x) + end)

# IO helpers
def get_int():
    return int(input())
def get_list_ints():
    return list(map(int, input().split()))
def get_char_list():
    s = input()
    return list(s[:len(s) - 1])
def get_tuple_ints():
    return tuple(map(int, input().split()))
def print_iterable(p):
    print(" ".join(map(str, p)))

def main():

    # code goes here
    # example:
    a, b = get_tuple_ints()
    l = [a + b, a - b]
    print(a + b)
    print_iterable(l)

    pass

if __name__ == '__main__':
    main()
