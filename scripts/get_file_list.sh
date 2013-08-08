get_file_list.pl -keys path,filename -cond 'filename~st_W,production=P09ig,filetype=daq_reco_MuDst,storage=nfs' -onefile -alls -delim / -limit 50
get_file_list.pl -keys path,filename -cond 'filename~st_W,library=SL11b,filetype=daq_reco_MuDst,storage=nfs' -onefile -alls -delim / -limit 50
get_file_list.pl -keys path,filename -cond 'filename~st_W,library=SL11b,filetype=daq_reco_MuDst,storage=nfs' -onefile -alls -delim / -limit 50

get_file_list.pl -keys path,filename -cond 'filename~st_W,filetype=daq_reco_MuDst,storage=hpss,runnumber=12043045'  -alls -delim / -limit 50

# for Run 11 data
# from the wiki page
get_file_list.pl -keys 'runnumber,path,filename' -cond 'path!~long,filename~st_W,production=P11id,filetype=daq_reco_mudst,storage=hpss ' -limit 0 -delim '/' 
# all
get_file_list.pl -keys 'runnumber' -cond 'production=P11id,trgsetupname~pp500_production_2011,sname1~st_W,storage=hpss' -limit 20 -delim '/' -distinc
# longitudinal
get_file_list.pl -keys 'runnumber' -cond 'production=P11id,trgsetupname~pp500_production_2011_long,sname1~st_W,storage=hpss' -limit 20 -delim '/' -distinc

# for Run 12 data
get_file_list.pl -keys 'runnumber' -cond 'production=P13ib,trgsetupname~pp500_production_2012,sname1~st_W,storage=nfs' -limit 20 -delim '/' -distinct

# for embedding
get_file_list.pl -keys path,filename,events -cond 'filename~st_zerobias_%,storage=hpss,trgsetupname=pp500_production_2011||pp500_production_2011_noeemc||pp500_production_2011_fms,trgname=zerobias,createtime~2011-%' -alls -delim / -limit 0
