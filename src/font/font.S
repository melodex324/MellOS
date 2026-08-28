.section .rodata
.global _binary_font_psf_start
.global _binary_font_psf_end

_binary_font_psf_start:
    .incbin "iso_02_f16.psf"

_binary_font_psf_end:

.section .note.GNU-stack,"",@progbits