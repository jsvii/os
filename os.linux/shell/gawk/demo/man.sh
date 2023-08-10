#!/bin/bash

set -x
# set -u

# ls ../deploy/database/scampaignportal/incremental-* | sort


ls ../deploy/database/scampaignportal/incremental* | sort | awk '{print("source", $0)}' | mysql -p -u

#for ARG in $(ls ../deploy/database/scampaignportal/incremental* | sort) ; do
#    echo $ARG;
#done
