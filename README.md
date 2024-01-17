# criPPLe
Windows kernel driver that removes Process Protection (PP) and Process Protection Light (PPL).

## This was made with the help of Zero-Point Security's ["Offensive Driver Development course"](https://training.zeropointsecurity.co.uk/courses/offensive-driver-development)

**This kernel driver removes the process protection from a given pid, it gets a pointer to the process's PS_PROTECTION struct and sets these values to 0.**

```
   _PS_PROTECTION

   +0x000 Level            : UChar
   +0x000 Type             : Pos 0, 3 Bits
   +0x000 Audit            : Pos 3, 1 Bit
   +0x000 Signer           : Pos 4, 4 Bits

```

**Dually this kernel driver can also make a process protected by setting the process's PS_PROTECTION struct values to values that suit your needs**

# Uses

- **Do note that PP is superior to PPL any PP can obtain full access to a PPL, never vice versa. You can set your process protection to a higher protection level than your target this could be very useful**

- **Stated in the course you can use this to disable the Process Protection light (PPL) from lsass.exe and allow mimikatz to do it's work**

- **Personally I used this to disable the Process Protection Light (PPL) from csrss.exe (to test other protected processes)**

- **I've only played with this for a little but so far I think maybe sending the pid of MsMpEng.exe (Microsoft Malware Protection Engine) which is also another (PPL) process may be a good idea I will see...**
