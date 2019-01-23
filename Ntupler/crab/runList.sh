#!/bin/bash

file=$1
isData=`echo $file | grep -c data`
isSig=`echo $file | grep -c sig`
isPriv=`echo $file | grep -c private`
is8X=`echo $file | grep -c 8X`
is9X=`echo $file | grep -c 9X`
is10X=`echo $file | grep -c 10X`
config=makingBacon_Data_25ns_MINIAOD.py
if [ $is8X -ne 0 ]; then
    config=makingBacon_Data_25ns_MINIAOD_8X.py
fi
if [ $isData -eq 0 ]; then
    config=makingBacon_Gen_25ns_MINIAOD.py
    if [ $isSig -eq 0 ] && [ $isPriv -eq 0 ]; then
        config=makingBacon_MC_25ns_MINIAOD_8X.py
        if [ $is8X -eq 0 ]; then
            config=makingBacon_MC_25ns_MINIAOD_9X.py
	    if [ $is9X -eq 0 ]; then
		config=makingBacon_MC_25ns_MINIAOD.py
	    fi
        fi
    fi
fi
if [ $is10X -ne 0 ]; then
    config=makingBacon_MC_25ns_MINIAOD_10X.py
fi


for x in `cat $file | grep -v "#"`; do 
    ext='_ext'
    isExt=`echo $x | grep -c ext`
    if [ $isExt -eq 0 ]; then
	ext=''
    fi



    label=`echo $x   | sed "s@/@ @g" | awk '{print $1_$2}'  | sed "s@-madgraphMLM-pythia8@@g" | sed "s@-@_@g"`
    if [ $isData -eq 0 ]; then
	label=`echo $x   | sed "s@/@ @g" | awk '{print $1}'  | sed "s@-madgraphMLM-pythia8@@g" | sed "s@-@_@g"`
    fi
    label=`echo $label | sed "s@TuneCUETP8M1_@@g"`
    label=`echo $label | sed "s@_13TeV-powheg@@g"`
    label=`echo $label | sed "s@DMV_NNPDF30_@@g"`
    label=`echo $label | sed "s@DMS_NNPDF30_@@g"`
    label=`echo $label | sed "s@_gSM-1p0_gDM-1p0@@g"`
    label=${label}${ext}
    if [ $is8X -ne 0 ]; then
        label=${label}_8X
    fi
    if [ $is10X -ne 0 ]; then
        label=${label}_10X
    fi
    label=${label}_try2
    #label=${label}_withPF
    #crab kill crab_projects/crab_${label}
    #crab status crab_projects/crab_${label}
    #crab report crab_projects/crab_${label}
    #crab resubmit crab_projects/crab_${label}
    #crab getoutput --dir crab_projects/crab_${label}
    #rm -r -f crab_projects/crab_${label}
    #crab extend crab_projects/crab_${label}
    #echo ${label} $x $config  1
    ./run.sh ${label} $x $config  1
done