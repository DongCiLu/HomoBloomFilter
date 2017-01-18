captain_str_tl = {'1-15 points', '16-30 points', '31-45 points', 'above 45 points'};
captain_str_comp_oh = {'Encryption at A', ...
    'Encryption at B', ...
    'Computation at Server', ...
    'Decryption at A'};
captain_str_comm_oh = {'A to Server', ...
    'B to Server', ...
    'Server to A'};
label_str_protocol = {'P1', 'P2', 'P3'};
label_str_p = {'0.1', '0.05', '0.02', '0.01', '0.005', '0.002', '0.001'};

figure(1); % compuatation overheads

for i = 1:4
    sp = subplot(1,4,i);
    bar_content = [protocol1_comp(i,:); protocol2_comp(i,:); protocol3_comp(i,:)];
    bar(bar_content);
    ylim(sp, [100 15000]);
    set(gca,'FontSize',24);
    set(gca,'XTickLabel',label_str_protocol);
    set(gca, 'YScale', 'log');
    % xlabel_hand=xlabel('Trajectory length');
    % set(xlabel_hand,'Fontname', 'Times New Roman', 'Fontsize', 24);
    if i == 1
        ylabel_hand=ylabel('Run time (ms)');
        set(ylabel_hand,'Fontname', 'Times New Roman', 'Fontsize', 24);
    end
    title(captain_str_tl(i));
end
% legend(captain_str_comp_oh,...
%     'Orientation', 'horizontal',...
%     'Fontsize', 24,...
%     'Position', [0 0 1 0.05]);
% legend boxoff;

figure(2); % communication overheads

for i = 1:4
    sp = subplot(1,4,i);
    bar_content = [protocol1_comm(i,:); protocol2_comm(i,:); protocol3_comm(i,:)];
    bar(bar_content);
    ylim(sp, [1 10000]);
    set(gca,'FontSize',24);
    set(gca,'XTickLabel',label_str_protocol);
    set(gca, 'YScale', 'log');
    % xlabel_hand=xlabel('Trajectory length');
    % set(xlabel_hand,'Fontname', 'Times New Roman', 'Fontsize', 24);
    if i == 1
        ylabel_hand=ylabel('Ciphertext size (KB)');
        set(ylabel_hand,'Fontname', 'Times New Roman', 'Fontsize', 24);
    end
    title(captain_str_tl(i));
end
% legend(captain_str_comm_oh,...
%     'Orientation', 'horizontal',...
%     'Fontsize', 24,...
%     'Position', [0 0 1 0.05]);
% legend boxoff;

figure(3); % accuracy & comp overheads
x_location = [0.1, 0.05, 0.02, 0.01, 0.005, 0.002, 0.001];
for i = 1:4
    subplot(1,4,i);
    plot_content = [protocol1_acc_comp(:,i), protocol2_acc_comp(:,i), protocol3_acc_comp(:,i)];   
    plot(x_location, plot_content(:,1), 'g-o', 'LineWidth', 2, 'Markers', 7); hold on;
    plot(x_location, plot_content(:,2), 'b--x', 'LineWidth', 2, 'Markers', 7); hold on;
    plot(x_location, plot_content(:,3), 'r:+', 'LineWidth', 2, 'Markers', 7);
    % hold on;
    set(gca,'FontSize',24);
    % set(gca,'XTickLabel',label_str_p);
    % set(gca, 'YScale', 'log');
    xlabel_hand=xlabel('False positive possibility');
    set(xlabel_hand,'Fontname', 'Times New Roman', 'Fontsize', 24);
    if i == 1
        ylabel_hand=ylabel('Run time (ms)');
        set(ylabel_hand,'Fontname', 'Times New Roman', 'Fontsize', 24);
    end
    legend(label_str_protocol);
    title(captain_str_comp_oh(i));
end

figure(4); % accuracy & comm overheads
x_location = [0.1, 0.05, 0.02, 0.01, 0.005, 0.002, 0.001];
for i = 1:3
    subplot(1,3,i);
    plot_content = [protocol1_acc_comm(:,i), protocol2_acc_comm(:,i), protocol3_acc_comm(:,i)];
    plot(x_location, plot_content(:,1), 'g-o', 'LineWidth', 2, 'Markers', 7); hold on;
    plot(x_location, plot_content(:,2), 'b--x', 'LineWidth', 2, 'Markers', 7); hold on;
    plot(x_location, plot_content(:,3), 'r:+', 'LineWidth', 2, 'Markers', 7);
    % hold on;
    set(gca,'FontSize',24);
    % set(gca,'XTickLabel',label_str_p);
    % set(gca, 'YScale', 'log');
    xlabel_hand=xlabel('False positive possibility');
    set(xlabel_hand,'Fontname', 'Times New Roman', 'Fontsize', 24);
    if i == 1
        ylabel_hand=ylabel('Ciphertext size (KB)');
        set(ylabel_hand,'Fontname', 'Times New Roman', 'Fontsize', 24);
    end
    legend(label_str_protocol);
    title(captain_str_comm_oh(i));
end