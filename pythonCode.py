


def multiply(a,b):
    print("CEJ: Will compute", a, "times", b)
    c = 0
    for i in range(0, a):
        c = c + b
    return c

def executePython(script):
    exec(script)