#
# mkplay.pl - generate the players for all the formats
#
# Could be done by hand; certainly could just be a batch file or shell script.
# Using perl makes it commonly useful, though.
#
# Also builds the splash screen.

# You can take this out unless you use Perforce source code control.
system("p4 edit player/h*");

&mkplay(1, 1);

&mkplay(3, 3);
&mkplay(3, 1);

&mkplay(4, 3);
&mkplay(4, 1);

&mkplay(5, 3);
&mkplay(5, 1);

&mkplay(6, 3);
&mkplay(6, 1);

system("p4 edit player/splash.zx7");
&mksplash;
system("p4 revert -a player/splash.zx7");

# You can take this out unless you use Perforce source code control.
system("p4 revert -a player/h*");

sub mkplay {
	($host, $format) = @_;
	$name = "h$host" . "f$format";
	system("perl genplayer.pl $host $format > player/$name.z");
	chdir("player");
	system("zmac -m $name.z");	# -m to expand macros in output
	unlink("$name.zx7");
	system("zx7 zout/$name.cim $name.zx7");
	chdir("..");
}

sub mksplash {
	open(SPLASH, "<splash.txt") || die "Can't read splash.txt: $!\n";
	open(TMP, ">player/tmp") || die "Can't write player/tmp: $!\n";
	while (<SPLASH>) {
		chomp;
		print TMP;
	}
	close SPLASH;
	close TMP;
	chdir("player");
	unlink("splash.zx7");
	system("zx7 tmp splash.zx7\n");
	unlink("tmp");
	chdir("..");
}
