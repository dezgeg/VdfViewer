#!/usr/bin/octave -qf
set(0, 'defaultfigurevisible', 'off'); % hide plot window
args = argv();
data = load(args{1}); % open input file
noanim = strcmp(args{3}, '-n');
n = data(1, 1);
grid on;
hold on;

for i = 0:(n-1)
		xs = data(:, 3 + 3*i);
		ys = data(:, 4 + 3*i);
		zs = data(:, 5 + 3*i);

		plot3(xs, ys, zs, sprintf('o-%d', i+1));
end
print(sprintf('output/%s.png', args{2}))
if noanim
	exit();
end
a = axis;
clf
m = rows(data);

for j = 1:m
		hold on;
		for i = 0:(n-1)
			xs = [data(1:j, 3 + 3*i)];
			ys = [data(1:j, 4 + 3*i)];
			zs = [data(1:j, 5 + 3*i)];

			axis(a);
			grid on;
			if j == 1
				plot3(xs, ys, zs, sprintf('.*%d', i+1));
			else
				plot3(xs, ys, zs, sprintf('-*%d', i+1));
			end
		end
		hold off;
		print(sprintf('output/%s-frame%05d.gif', args{2}, j))
		clf;
		j
end
