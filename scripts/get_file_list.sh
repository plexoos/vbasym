get_file_list.pl -keys path,filename -cond 'filename~st_W,production=P09ig,filetype=daq_reco_MuDst,storage=nfs' -onefile -alls -delim / -limit 50
get_file_list.pl -keys path,filename -cond 'filename~st_W,library=SL11b,filetype=daq_reco_MuDst,storage=nfs' -onefile -alls -delim / -limit 50
get_file_list.pl -keys path,filename -cond 'filename~st_W,library=SL11b,filetype=daq_reco_MuDst,storage=nfs' -onefile -alls -delim / -limit 50

get_file_list.pl -keys path,filename -cond 'filename~st_W,filetype=daq_reco_MuDst,storage=hpss,runnumber=12043045'  -alls -delim / -limit 50

# for embedding
get_file_list.pl -keys path,filename,events -cond 'filename~st_zerobias_%,storage=hpss,trgsetupname=pp500_production_2011||pp500_production_2011_noeemc||pp500_production_2011_fms,trgname=zerobias,createtime~2011-%' -alls -delim / -limit 0
