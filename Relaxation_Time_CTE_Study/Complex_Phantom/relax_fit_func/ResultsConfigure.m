% close all;
% clear all;
%[fln,flp]=uigetfile('*.txt') % user interface to select text file

function outfile=ResultsConfigure(Inputfile)
fid = fopen(Inputfile,'r');  % Open text file
InputText=textscan(fid,'%s',2,'delimiter','\n');  % Read strings delimited
                                                  % by a carriage return  
                                                  
                                                 

Intro=InputText{1};
disp(Intro);
Block = 1;                                         % Initialize block index
while (~feof(fid))                                   % For each block:

    sprintf('Block: %s', num2str(Block));            % Display block number
    InputText=textscan(fid,'%s',2,'delimiter','\n'); % Read header line
    HeaderLines{Block,1}=InputText{1};
    titles{Block}=HeaderLines{Block};
    

    InputText=textscan(fid,'Num SNR=%f');            % Read parameter value
    NumCols=InputText{1};

    FormatString=repmat('%f',1,NumCols);             % Create format string
                                                     % based on parameter
    InputText=textscan(fid,FormatString, ...         % Read data block
                       'delimiter',',');

    Data{Block,1}=cell2mat(InputText);              % Convert to numerical
                                                     % array from cell
    [NumRows,NumCols]=size(Data{Block});             % Size of table
    %disp(cellstr([xlate('Table data size: '), ...
     %             num2str(NumRows) ' x ' num2str(NumCols)]));
    disp(' ');                                       % New line

    eob=textscan(fid,'%s',1,'delimiter','\n');       % Read and discard EOB
                                                     % marker (Here, 'EOF')
                                                     
    Data4byN{Block}=reshape(Data{Block},[4,length(Data{Block})/4])'; %reshape data to 4 by N matrix
                                                     %Note:above line is
                                                     %hardwired but suits
                                                                         %current needs
                                                     
    Block = Block+1;                               % Increment block index
    
  
    
end

numberofblocks = Block-1 ;

for i = 1:numberofblocks,
        temp = HeaderLines{i}(1);
        [str, rem] = strtok(temp, ' ');
        [str, rem] = strtok(rem, ' ');
        tm(i) = str2double(rem);
end

outfile=[Inputfile(1:end-4),'_dat.mat'];

save(outfile,'Data4byN','tm')

