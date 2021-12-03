 pushfq 
 push   rsi
 push   rdi
 push   rbp
 push   rax
 push   rbx
 push   rcx
 push   rdx
 push   rsp
 push   r8
 push   r9
 push   r10
 push   r11
 push   r12
 push   r13
 push   r14
 push   r15
 xor    r9, r9
 xor    r10, r10
 xor    r11, r11
 xor    r12, r12
 xor    r13, r13
 xor    r14, r14
 xor    r15, r15
 xor    rsi, rsi

# pointers
 movabs rbx, 0xaaaaaaaaaaaaaaaa     # world
 movabs rcx, 0xbbbbbbbbbbbbbbbb     # presults
 movabs r8, 0xcccccccccccccccc      # ptr read proxy

# read uworld
 mov    rax, rbx
 call   r8

# validate
 cmp    rax, 0
 je     invalid

# persistent lvl
 mov    rbx, rax
 add    rax, 0xdddd
 call   r8

# validate
 cmp    rax, 0
 je     invalid

 mov    r9, rax

 # actorsarray
 add    rax, 0xcafe
 call   r8
 mov    qword ptr [rcx], rax
 
 mov    rax, r9

 # actors count
 add    rax, 0xbabe
 call   r8
 mov    dword ptr [rcx + 8], eax

# gameinstance
ginstance: 
 mov    rax, rbx
 add    rax, 0xeeee
 call   r8

# validate
 cmp    rax, 0
 je     actors

# localplayer
 add    rax, 0x40
 call   r8

# validate
 cmp    rax, 0
 je     actors

# localplayer deref
 call   r8

# validate
 cmp    rax, 0
 je     actors

# player controller
 add    rax, 0x38
 call   r8

# validate
 cmp    rax, 0
 je     actors

# store player controller
 mov    r15, rax

# pawn
 add    rax, 0x440
 call   r8

# validate
 cmp    rax, 0
 je     actors

 mov    qword ptr [rcx + 0x4c], rax

# playerstate
 add    rax, 0x3d0
 call   r8
 mov    qword ptr [rcx + 0xc], rax
 
# cameramanager
camera:
 mov    rax, r15
 add    rax, 0x458
 call   r8

# validate
 cmp    rax, 0
 je     actors

 mov    r14, rax

# camera position
 add    rax, 0x1220
 call   r8
 mov    dword ptr [rcx + 0x14], eax
 mov    rax, r14
 add    rax, 0x1224
 call   r8
 mov    dword ptr [rcx + 0x18], eax
 mov    rax, r14
 add    rax, 0x1228
 call   r8
 mov    dword ptr [rcx + 0x1c], eax

# camera rotation
 mov    rax, r14
 add    rax, 0x122c
 call   r8
 mov    dword ptr [rcx + 0x20], eax
 mov    rax, r14
 add    rax, 0x1230
 call   r8
 mov    dword ptr [rcx + 0x24], eax
 mov    rax, r14
 add    rax, 0x1234
 call   r8
 mov    dword ptr [rcx + 0x28], eax

  # camera fov
 mov    rax, r14
 add    rax, 0x1238
 call   r8
 mov    dword ptr [rcx + 0x2c], eax

 actors:
 mov    rbx, qword ptr [rcx]
 mov    r15, rcx

 xor    r9, r9
 
 # validate actorsarray
 cmp    rbx, 0
 je     invalid

 # ptr entitycache
 movabs rcx, 0xdddddddddddddddd

 # legit deref
 mov    rdx, r8
 xor    r8, r8

 # loop
 iteration:

 # get actor entries
 imul   r8, rsi, 8

 # use pactor from r10
 mov    r10, rbx

 # calc actors address
 add    r10, r8

# read actor addy
 mov    rax, r10
 call   rdx

# validate
 movabs r12, 0x7fffffffffff
 cmp    rax, r12
 ja     skip
 movabs r12, 0x1000000000
 cmp    rax, r12
 jb     skip

 mov    r11, rax

# calc addy of pentitycache entry
 imul   r8, rsi, 0x34
 mov    r10, rcx
 add    r10, r8

 # unique id
 mov    rax, r11
 add    rax, 0x38
 call   rdx

 mov    r12d, 0x11e0101
 cmp    eax, r12d
 jne    skip
 mov    qword ptr [r10], rax

 # bdormant
 mov    rax, r11
 add    rax, 0xd8
 call   rdx
 mov    qword ptr [r10 + 0x4], rax

 # damage handler
 mov    rax, r11
 add    rax, 0x978
 call   rdx

 # health 
 mov    r13, rax
 add    rax, 0x1b0
 call   rdx
 mov    qword ptr [r10 + 0x8], rax

 # isalive
 mov    rax, r13
 add    rax, 0x1a9
 call   rdx
 mov    qword ptr [r10 + 0xc], rax

 # playerstate
 mov    rax, r11
 add    rax, 0x3d0
 call   rdx
 mov    qword ptr [r10 + 0x10], rax

 # mesh 
 mov    rax, r11
 add    rax, 0x410
 call   rdx
 mov    qword ptr [r10 + 0x18], rax

 cmp rax, 0x0
 je invalid

 # bone count
 add rax, 0x560
 call rdx
 mov    dword ptr [r10 + 0x30], eax

 # root component
 mov    rax, r11
 add    rax, 0x210
 call   rdx
 mov    qword ptr [r10 + 0x20], rax

 # object ptr
 mov    qword ptr [r10 + 0x28], r11

 skip:
 inc    rsi

# actors count
 mov    r13d, dword ptr [r15 + 0x8]

 # check actors count / iteration
 cmp    r13d, 0x1600
 ja     invalid
 cmp    esi, r13d
 jb     iteration

 invalid:
 pop    r15
 pop    r14
 pop    r13
 pop    r12
 pop    r11
 pop    r10
 pop    r9
 pop    r8
 pop    rsp
 pop    rdx
 pop    rcx
 pop    rbx
 pop    rax
 pop    rbp
 pop    rdi
 pop    rsi
 popfq  
 movabs rax, 0xffffffffffffffff
 jmp    rax