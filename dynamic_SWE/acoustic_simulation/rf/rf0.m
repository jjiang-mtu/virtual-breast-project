% final step
clear all;
close all;

fs=50e6;
no_lines=200;
for N_folder=1:10
    for step_index = 280:2:440
        cmd=['load /home/admin/SWS_breast_model/SSI_inclusion/track/loop',num2str(N_folder),'/rf_ln_step',num2str(step_index),'.mat'];
        eval(cmd);
        for i = 1:no_lines
            rf_data = rf_env{1, i};
            tstart = rf_time{1, i};
            rf_env_new = [zeros(round(tstart*fs),1); rf_data];
            env(1:max(size(rf_env_new)),i)=rf_env_new;
        end
        data = env;
        cmd = ['save step_',num2str(N_folder),'_',num2str(step_index),'.mat data'];
        eval(cmd);
    end
end
