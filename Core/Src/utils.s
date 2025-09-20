.syntax unified

.global _close
.global _isatty
.global _lseek
.global Set_BitBandVal
.global Get_BitBandVal



    .section  .text._close
    .type _close, %function
_close:
  ldr r1, =-1
  bx lr
  .size  _close, .-_close


    .section  .text._isatty
    .type _isatty, %function
_isatty:
  ldr r1, =1
  bx lr
  .size  _isatty, .-_isatty


    .section  .text._lseek
    .type _lseek, %function
_lseek:
  ldr r1, =0
  bx lr
  .size  _lseek, .-_lseek


    .section  .text.Set_BitBandVal
    .type Set_BitBandVal, %function
Set_BitBandVal:
  str r1, [r0]
  bx lr
  .size  Set_BitBandVal, .-Set_BitBandVal


    .section  .text.Get_BitBandVal
    .type Get_BitBandVal, %function
Get_BitBandVal:
  ldr r0, [r0]
  bx lr
  .size  Get_BitBandVal, .-Get_BitBandVal


