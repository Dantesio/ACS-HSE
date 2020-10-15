; Variant 4, Udachin Daniil, BSE191
; Create and print new vector B
; with the following values
; depends on values of vector A:
; B[i] = 1,  A[i]>0
; B[i] = -1, A[i]<0
; B[i] = 0,  A[i]=0

format PE console
entry start

include 'win32a.inc'

;______________________________________________________________________________
; Data
;______________________________________________________________________________

section '.data' data readable writable

        strVecSize     db 'Enter size of vector A: ', 0
        strIncorSize   db 'Incorrect size of vector = %d', 10, 0
        strVecElemI    db '[%d]? ', 0
        strScanInt     db '%d', 0

        strVecA        db 'Vector A:', 10, 0
        strVecB        db 'Vector B:', 10, 0
        strVecElemOut  db '[%d] = %d', 10, 0
        strExit        db 'Exiting process. Press any key...', 10, 0

        vecSize        dd 0
        i              dd ?
        tmp            dd ?
        tmpStack       dd ?

        vecA           rd 100
        vecB           rd 100
;______________________________________________________________________________
; Main
;______________________________________________________________________________

section '.code' code readable executable

start:
        call VectorInput
        call FillMemoryWithVectors
        call VectorFillLoop
        push strVecB
        call [printf]
        call VectorPrint
finish:
        push strExit
        call [printf]
        call [getch]

        push 0
        call [ExitProcess]

;------------------------------------------------------------------------------
; Input func
VectorInput:
        push strVecSize
        call [printf]
        add esp, 4

        push vecSize
        push strScanInt
        call [scanf]
        add esp, 8

        mov eax, [vecSize]
        cmp eax, 0
        jg  getVector

        push [vecSize]
        push strIncorSize
        call [printf]
        call [getch]

        push 0
        call [ExitProcess]

;------------------------------------------------------------------------------
; Getting vector func
getVector:
        xor ecx, ecx          ; ecx = 0
        mov ebx, vecA         ; ebx = &vecA

;------------------------------------------------------------------------------
; Loop func
getVecLoop:
        mov [tmp], ebx
        cmp ecx, [vecSize]    ; Comparing i value and vecSize value for the loop checking
        jge endInputVector    ; Exit if end of vector

        mov [i], ecx
        push ecx
        push strVecElemI
        call [printf]
        add  esp, 8

        push ebx
        push strScanInt
        call [scanf]
        add  esp, 8

        mov ecx, [i]
        inc ecx
        mov ebx, [tmp]
        add ebx, 4
        jmp getVecLoop        ; Repeat

;--------------------
; Exit
endInputVector:
        mov ebx, vecA
        xor ecx, ecx
        ret

;------------------------------------------------------------------------------
; Input func
FillMemoryWithVectors:
        xor ecx, ecx          ; ecx = 0
        mov ebx, vecA         ; ebx = &vecA
        mov edx, vecB
        ret

VectorFillLoop:
        cmp ecx, [vecSize]    ; Comparing i value and vecSize value for the loop checking
        jge endOfVector       ; Exit if end of vector

        mov [i], ecx
        mov eax, 0
        cmp [ebx], eax        ; Comparing A[i] and filling B[i]

        jg Insert1            ; Insert 1
        je Insert0            ; Insert 0
        jl InsertNeg1         ; Insert -1

        AssemblersGarbage:
            add ebx, 4
            add edx, 4
            inc ecx

        jmp VectorFillLoop

;---------------------
; Insert func 1
Insert1:
        mov eax, 1
        mov [edx], eax

        jmp AssemblersGarbage

;---------------------
; Insert func 0
Insert0:
        mov eax, 0
        mov [edx], eax

        jmp AssemblersGarbage

;---------------------
; Insert func -1
InsertNeg1:
        mov eax, -1
        mov [edx], eax

        jmp AssemblersGarbage

;---------------------
; End func
endOfVector:
        ret

;------------------------------------------------------------------------------
; Output Vector B func
VectorPrint:
        mov [tmpStack], esp
        xor ecx, ecx          ; ecx = 0
        mov ebx, vecB         ; ebx = &vecB

printVecLoop:
        mov [tmp], ebx
        cmp ecx, [vecSize]    ; Comparing i value and vecSize value for the loop checking
        je endOutputVector
        mov [i], ecx

        ; Print B[i]
        push dword [ebx]
        push ecx
        push strVecElemOut
        call [printf]

        mov ecx, [i]
        inc ecx
        mov ebx, [tmp]
        add ebx, 4
        jmp printVecLoop      ; Repeat

endOutputVector:
        mov esp, [tmpStack]
        ret

;______________________________________________________________________________
; Libraries and imports
;______________________________________________________________________________

section '.import' import data readable writable

        library kernel, 'kernel32.dll',\
                msvcrt, 'msvcrt.dll',\
                user32, 'user32.dll'

        include 'api\kernel32.inc'
            import kernel,\
                ExitProcess, 'ExitProcess'

        include 'api\user32.inc'
            import msvcrt,\
                printf, 'printf',\
                scanf, 'scanf',\
                getch, '_getch'

























