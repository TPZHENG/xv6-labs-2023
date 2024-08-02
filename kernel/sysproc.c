#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;


  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}


#ifdef LAB_PGTBL
int
sys_pgaccess(void)
{
  // lab pgtbl: your code here.
  pagetable_t u_pt = myproc()->pagetable;
  uint64 fir_addr, mask_addr;
  int ck_siz;
  int mask = 0;
  argaddr(0, &fir_addr);
  argint(1, &ck_siz);
  argaddr(2, &mask_addr);

  if(ck_siz > 32){
      return -1;
  }

  pte_t* fir_pte = walk(u_pt, fir_addr, 0);

  for(int i = 0; i < ck_siz; i++){
      if((fir_pte[i] & PTE_A) && (fir_pte[i] & PTE_V)){
          mask |= (1 << i);
          fir_pte[i] ^= PTE_A; // 复位
      }
  }

  copyout(u_pt, mask_addr, (char *)&mask, sizeof(uint));

  return 0;
}
#endif

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
