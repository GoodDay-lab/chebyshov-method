" Локальные настройки для проекта
set path=include,src
set tags=./tags,tags;

" Компиляция тегов при сохранении
autocmd BufWritePost *.c,*.h,*.cpp,*.hpp silent! !ctags -R . &
