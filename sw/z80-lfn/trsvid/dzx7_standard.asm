








dzx7_standard:
        ld      a, $80
dzx7s_copy_byte_loop:
        ldi                             
dzx7s_main_loop:
        call    dzx7s_next_bit
        jr      nc, dzx7s_copy_byte_loop 


        push    de
        ld      bc, 0
        ld      d, b
dzx7s_len_size_loop:
        inc     d
        call    dzx7s_next_bit
        jr      nc, dzx7s_len_size_loop


dzx7s_len_value_loop:
        call    nc, dzx7s_next_bit
        rl      c
        rl      b
        jr      c, dzx7s_exit           
        dec     d
        jr      nz, dzx7s_len_value_loop
        inc     bc                      


        ld      e, (hl)                 
        inc     hl
        defb    $cb, $33                
        jr      nc, dzx7s_offset_end    
        ld      d, $10                  
dzx7s_rld_next_bit:
        call    dzx7s_next_bit
        rl      d                       
        jr      nc, dzx7s_rld_next_bit  
        inc     d                       
        srl	d			
dzx7s_offset_end:
        rr      e                       


        ex      (sp), hl                
        push    hl                      
        sbc     hl, de                  
        pop     de                      
        ldir
dzx7s_exit:
        pop     hl                      
        jr      nc, dzx7s_main_loop
dzx7s_next_bit:
        add     a, a                    
        ret     nz                      
        ld      a, (hl)                 
        inc     hl
        rla
        ret


