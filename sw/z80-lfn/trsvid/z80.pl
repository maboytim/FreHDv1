#
# z80.pl - Z-80 mini assembler routines
#
# Idea is to allow assembly of a line at a time which some kind of "fixup"
# strategy that allows for some degree of labels and expressions.  Perhaps
# just a second pass?
# 
# Make easy to get under the covers.  Top level just looks for labels
# and defines them.  "$" is the special label used here.  All labels in $z{}.
# Then breaks the line into opcode and args.  Calls z_asm(opcode, arg, arg).
# High-level paren detection could be nice.
# z_asm() returns assembled data and updates "$", "t" and "m" variables.
# Or maybe returns (data, cycles).
# Probably have z_inf() helper that just gets the info without updating globals.
# z_asm() puts stuff into "memory" as it were.  Just adding to a string.
# Caller can deal with "org" and friends, though we might have a subroutine
# to look for overlap.
#
# Add ":" extension to allow multiple statements per line.
#
# Agressive about reserved words for registers.  And (HL) -> M?
#
# Eh, maybe just plough them into large constants.
# So each subroutine does not have to evaluate them, just range check.
#
# Lay down a structure, but don't get sidetracked.  Wanna have M3 audio!

# Globals:
# 	z_line 	- current line being assembled
#	z_pc	- current assembly address
#	z_ld	- current load address (for phase stuff, maybe)
#	z_t		- current T-State count
#	z_m		- current M1-State count

$regbase =  100 * 1000 * 1000;
$rpbase =   110 * 1000 * 1000;
$bcdebase = 120 * 1000 * 1000;
$condbase = 200 * 1000 * 1000;
$miscbase = 210 * 1000 * 1000;

%var = (
	'b',	$regbase + 0,
	'c',	$regbase + 1,
	'd',	$regbase + 2,
	'e',	$regbase + 3,
	'h',	$regbase + 4,
	'l',	$regbase + 5,
	'M',	$regbase + 6,		# (hl)
	'a',	$regbase + 7,

	'bc',	$rpbase + 0,
	'de',	$rpbase + 1,
	'hl',	$rpbase + 2,
	'sp',	$rpbase + 3,
	'af',	$rpbase + 3,

	'K',	$bcdebase + 0,		# (bc)
	'L',	$bcdebase + 0x10,	# (de)

	'nz',	$condbase + 0 * 8,
	'z',	$condbase + 1 * 8,
	'nc',	$condbase + 2 * 8,
	'cs',	$condbase + 3 * 8,	# avoid collision with C register
	'po',	$condbase + 4 * 8,
	'pe',	$condbase + 5 * 8,
	'p',	$condbase + 6 * 8,
	'm',	$condbase + 7 * 8,

	'i',	$miscbase + 0,
	'r',	$miscbase + 1,

	'$',	0,
);


sub asm
{
	my($t, $m, $ttot, $mtot) = (0, 0, 0, 0);
	my($code) = '';
	foreach (split("\n", $_[0]))
	{
		s/;.*//;
		next if /^\s*$/;
		$z_line = $_; # for easy error echo

		#print "xxx: $z_line\n";

		# label?

		s/^\s+//;
		tr/[A-Z]/[a-z]/;
		s/\$([0-9a-f]+)/hex($1)/eg;
		@arg = split(/\s+,?|,/);
		#print join('_', @cmd), "\n";
		$op = shift @arg;

		$arg[0] = 'K' if $arg[0] eq '(bc)';
		$arg[1] = 'K' if $arg[1] eq '(bc)';
		$arg[0] = 'L' if $arg[0] eq '(de)';
		$arg[1] = 'L' if $arg[1] eq '(de)';
		$arg[0] = 'M' if $arg[0] eq '(hl)';
		$arg[1] = 'M' if $arg[1] eq '(hl)';

		if ($arg[0] =~ /^\(/ && $arg[0] =~ /\)$/) {
			$arg[0] = substr($arg[0], 1, length($arg[0]) - 2);
			$op = "${op}_st";
		}
		if ($arg[1] =~ /^\(/ && $arg[1] =~ /\)$/) {
			$arg[1] = substr($arg[1], 1, length($arg[1]) - 2);
			$op = "${op}_ld";
		}

		foreach (@arg) {
			s/\b([a-z_][a-z_0-9]*)\b/&getvar($1)/ieg;
			s/\$/$var{'$'}/g;
		}

		#print $op, ' ', join(' ', @arg), "\n";

		defined($asm{$op}) || &bad("Undefined mnemonic '$op'");

		($bin, $min_extra, $wait) = $asm{$op}(@arg);
		$min = int($min_extra);
		$extra = int(($min_extra - $min) * 10);
		$min += $opwait{$op};  # cheap hack to allow video write wait states

		$m = 1;
		$op = unpack('C', $bin);
		$m += $op == 0xCB || $op == 0xDD || $op == 0xED || $op == 0xFD;

		$t = $min + $wait;

		$var{'$'} += length($bin);
		$var{'t'} += $t;
		$var{'m'} += $m;

		$ttot += $t;
		$mtot += $m;

		#printf "%2d ", $t;
		#printf "%04x ", $var{'$'};
		#print unpack('H*', $bin), "\t$z_line\t$min + $extra\n";
	}
	return ($code, $ttot, $mtot);
}

sub getvar
{
	return $var{$_[0]} if defined $var{$_[0]};
	return 0;
}

sub bad
{
	print "Error in: $z_line\n";
	print "$_[0]\n";
	exit;
}

sub addr
{
	return pack('v', $_[0]);
}

sub ex
{
	@_ == 2 || &bad;
	return (pack('C', 0x08), 4) if $_[0] == $var{'af'} && $_[1] == $var{'af'};
	return (pack('C', 0xeb), 4) if $_[0] == $var{'de'} && $_[1] == $var{'hl'};
	&bad;
}

sub ex_st
{
	@_ == 2 || &bad;
	($_[0] == $var{'sp'} && $_[1] == $var{'hl'}) || &bad;
	return (pack('C', 0xe3), 19);
}

sub exx
{
	@_ == 0 || &bad;
	return (pack('C', 0xd9), 4);
}

sub nop
{
	@_ == 0 || &bad;
	return (pack('C', 0x00), 4);
}

sub ini
{
	@_ == 0 || &bad;
	return (pack('C*', 0xed, 0xa2), 16, $wait{$regval{'c'}});
}

sub in_ld
{
	@_ == 2 || &bad;
	($_[0] >= $var{'b'} && $_[0] <= $var{'a'}) || &bad;
	$_[0] != $var{'M'} || &bad;
	return (pack('C*', 0xed, 0x40 | ($_[0] << 3)), 12, $wait{$regval{'c'}})
		if $_[1] == $var{'c'};
	$_[0] == $var{'a'} || &bad;
	return (pack('C*', 0xdb, $_[1]), 11, $wait{$_[1]});
}

sub ld
{
	@_ == 2 || &bad;
	$extra = 0;
	$extra += 3 * ($_[0] == $var{'M'});
	$extra += 3 * ($_[1] == $var{'M'});
	&bad if $extra == 6;

	return (pack('C*', 0xed, 0x47), 9) if $_[0] == $var{'i'} && $_[1] == $var{'a'};
	return (pack('C*', 0xed, 0x4f), 9) if $_[0] == $var{'r'} && $_[1] == $var{'a'};
	return (pack('C*', 0xed, 0x57), 9) if $_[0] == $var{'a'} && $_[1] == $var{'i'};
	return (pack('C*', 0xed, 0x5f), 9) if $_[0] == $var{'a'} && $_[1] == $var{'r'};

	return (pack('C', 0x02), 7) if $_[0] == $var{'K'} && $_[1] == $var{'a'};
	return (pack('C', 0x0a), 7) if $_[0] == $var{'a'} && $_[1] == $var{'K'};
	return (pack('C', 0x12), 7) if $_[0] == $var{'L'} && $_[1] == $var{'a'};
	return (pack('C', 0x1a), 7) if $_[0] == $var{'a'} && $_[1] == $var{'L'};

	return (pack('C*', 0x06 | ($_[0] << 3), $_[1]), 7 + $extra)
			if $_[0] >= $var{'b'} && $_[0] <= $var{'a'} &&
			   $_[1] >= -128 && $_[1] <= 255;

	return (pack('C', 0x40 | ($_[0] << 3) | $_[1]), 4 + $extra)
			if $_[0] >= $var{'b'} && $_[0] <= $var{'a'} &&
			   $_[1] >= $var{'b'} && $_[1] <= $var{'a'};

	return (pack('C', 0x01 | ($_[0] << 4)) . &addr($_[1]), 10)
		if $_[0] >= $var{'bc'} && $_[0] <= $var{'de'};

	&bad("unimplemented ld");
}

sub ld_st
{
	@_ == 2 || &bad;

	return (pack('C', 0x32) . &addr($_[0]), 13) if $_[1] eq $var{'a'};
	return (pack('C', 0x3a) . &addr($_[1]), 13) if $_[0] eq $var{'a'};

	&bad("unimplemented ld (x),y");
}

sub alu8
{
	shift if $_[0] == $var{'a'} && @_ > 2;
	@_ == 2 || &bad;
	return (pack('C*', 0xc6 | $_[1], $_[0]), 7)
		if $_[0] >= -128 && $_[0] <= 255;
	($_[0] >= $var{'b'} && $_[0] <= $var{'a'}) || &bad;
	return (pack('C', $_[1] | $_[0]), 4 + 3 * ($_[0] == $var{'M'}));
}

sub add { return &alu8(@_, 0x80); }
sub adc { return &alu8(@_, 0x88); }
sub sub { return &alu8(@_, 0x90); }
sub sbc { return &alu8(@_, 0x98); }
sub and { return &alu8(@_, 0xa0); }
sub xor { return &alu8(@_, 0xa8); }
sub or { return &alu8(@_, 0xb0); }
sub cp { return &alu8(@_, 0xb8); }

sub dec
{
	@_ == 1 || &bad;
	return (pack('C', 0xb | ($_[0] << 4)), 6) if $_[0] >= $var{'bc'} && $_[0] <= $var{'sp'};
	$_[0] >= $var{'b'} && $_[0] <= $var{'a'};
	return (pack('C', 5 | ($_[0] << 3)), 4 + 3 * ($_[0] == $var{'M'}));
}

sub inc
{
	@_ == 1 || &bad;
	return (pack('C', 0x3 | ($_[0] << 4)), 6) if $_[0] >= $var{'bc'} && $_[0] <= $var{'sp'};
	$_[0] >= $var{'b'} && $_[0] <= $var{'a'};
	return (pack('C', 4 | ($_[0] << 3)), 4 + 3 * ($_[0] == $var{'M'}));
}

sub out_st
{
	@_ == 2 || &bad;
	($_[1] >= $var{'b'} && $_[1] <= $var{'a'}) || &bad;
	$_[1] != $var{'M'} || &bad;
	return (pack('C*', 0xed, 0x41 | ($_[1] << 3)), 12) if $_[0] == $var{'c'};
	$_[1] == $var{'a'} || &bad;
	return (pack('C*', 0xd3, $_[0]), 11);
}

sub jr
{
	return (pack('C*', 0x18, 0), 12) if @_ == 1;
	@_ == 2 || &bad;
	$_[0] = $var{'cs'} if $_[0] == $var{'c'};
	$_[0] == $var{'cs'} ||
	$_[0] == $var{'nc'} ||
	$_[0] == $var{'z'} ||
	$_[0] == $var{'nz'} || &bad;
	return (pack('C*', 0x20 | $_[0], 0), 7.5);
}

sub jp
{
	if (@_ == 1) {
		return (pack('C', 0xe9), 4) if $_[0] == $var{'M'};
		return (pack('C', 0xc3) . &addr($_[0]), 10);
	}
	@_ == 2 || &bad;
	$_[0] = $var{'cs'} if $_[0] == $var{'c'};
	($_[0] >= $var{'nz'} && $_[0] <= $var{'m'}) || &bad;
	return (pack('C', 0xc0 | $_[0] & 0x38) . &addr($_[1]), 10);
}

sub ret
{
	return (pack('C', 0xc9), 10) if @_ == 0;
	@_ == 1 || &bad;
	$_[0] = $var{'cs'} if $_[0] == $var{'c'};
	($_[0] >= $var{'nz'} && $_[0] <= $var{'m'}) || &bad;
	return (pack('C', 0xc0 | $_[0] & 0x38), 5.6);
}

%asm = (
	'add',		\&add,
	'adc',		\&adc,
	'sub',		\&sub,
	'sbc',		\&sbc,
	'and',		\&and,
	'xor',		\&xor,
	'or',		\&or,
	'cp',		\&cp,
	'dec',		\&dec,
	'inc',		\&inc,

	'ld',		\&ld,
	'ld_st',	\&ld_st,

	'ex',		\&ex,
	'ex_st',	\&ex_st,
	'exx',		\&exx,
	'nop',		\&nop,

	'ini',		\&ini,
	'in_ld',	\&in_ld,
	'out_st',	\&out_st,

	'jr',		\&jr,
	'jp',		\&jp,
	'ret',		\&ret,
);

1;
