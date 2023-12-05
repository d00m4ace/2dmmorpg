if not exist "output\img_import\" mkdir output\img_import\

call gen_netpackets.bat 
call img_chrs.bat 
call img_i16.bat 
call img_mobs.bat