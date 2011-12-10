env = Environment(CCFLAGS = '-m32 -Wall')

sysfuck = env.Program('sysfuck', ['sysfuck.c', 'callbacks.c'])
str_to_syscall = env.Command('str_to_syscall.c', '', "./gen_str_to_syscall.sh")
Depends(sysfuck, str_to_syscall)
