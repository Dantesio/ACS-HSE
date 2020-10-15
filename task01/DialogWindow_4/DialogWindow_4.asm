include 'win32ax.inc'

.code
start:
        invoke MessageBox, HWND_DESKTOP, 'Вы опоздали со здачей домашнего задания?', invoke GetCommandLine, MB_YESNO
        .if eax = IDYES
            invoke MessageBox, HWND_DESKTOP, 'Получите оценку с понижающим коэффициентом', 'Александр Иванович говорит...', MB_OK
        .endif
        .if eax = IDNO
            invoke MessageBox, HWND_DESKTOP, '10. Без комментариев.',  'Молодцы.', MB_OK
        .endif


        invoke ExitProcess, 0
.end start