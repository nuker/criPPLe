# criPPLe
Windows kernel driver that removes Process Protection (PP) and Process Protection Light (PPL).

# This was made with the help of Zero-Point Security's ["Offensive Driver Development course"](https://training.zeropointsecurity.co.uk/courses/offensive-driver-development)

**This kernel driver removes the process protection from a given pid, and sets the fields of the process's PS_PROTECTION struct values to all 0**
**Dually this kernel driver can also make a process protected by setting the process's PS_PROTECTION struct values to custom values that suit your needs**

# Uses

- **Stated in the course work you can use this to disable the Process Protection light (PPL) from lsass.exe and allow mimikatz to do it's work**

- **Personally I used to to disable the Process Protection Light (PPL) from csrss.exe (Just to test another protected process)**

- **I've only played with this for a little but so far I found maybe sending MsMpEng.exe which is also another (PPL) process**
