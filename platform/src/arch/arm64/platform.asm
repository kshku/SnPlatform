AREA |.text|, CODE, READONLY

EXPORT sn_cntvct_el0

sn_cntvct_el0 PROC
    mrs x0, cntvct_el0
    ret
    ENDP

END
