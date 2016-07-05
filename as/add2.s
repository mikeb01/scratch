section    .text
global _fetch_and_add

_fetch_and_add:
  lock xadd [rdi], rsi
  mov       rax, rsi
  ret
