#include <stdbool.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>
#include <xtables.h>
#include "xt_ipp2p.h"
#define param_act(t, s, f) param_act((t), "ipp2p", (s), (f))

static void ipp2p_mt_help(void)
{
	printf(
	"IPP2P v%s options:\n"
	"  --edk    [tcp,udp]  All known eDonkey/eMule/Overnet packets\n"
	"  --dc     [tcp]      All known Direct Connect packets\n"
	"  --kazaa  [tcp,udp]  All known KaZaA packets\n"
	"  --gnu    [tcp,udp]  All known Gnutella packets\n"
	"  --bit    [tcp,udp]  All known BitTorrent packets\n"
	"  --apple  [tcp]      All known AppleJuice packets\n"
	"  --winmx  [tcp]      All known WinMX\n"
	"  --soul   [tcp]      All known SoulSeek\n"
	"  --ares   [tcp]      All known Ares\n\n"
	"EXPERIMENTAL protocols (please send feedback to: ipp2p@ipp2p.org) :\n"
	"  --mute   [tcp]      All known Mute packets\n"
	"  --waste  [tcp]      All known Waste packets\n"
	"  --xdcc   [tcp]      All known XDCC packets (only xdcc login)\n\n"
	"DEBUG SUPPPORT, use only if you know why\n"
	"  --debug             Generate kernel debug output, THIS WILL SLOW DOWN THE FILTER\n"
	"\nIPP2P was intended for TCP only. Due to increasing usage of UDP we needed to change this.\n"
	"You can now use -p udp to search UDP packets only or without -p switch to search UDP and TCP packets.\n"
	"\nSee README included with this package for more details or visit http://www.ipp2p.org\n"
	"\nExamples:\n"
	" iptables -A FORWARD -m ipp2p --ipp2p -j MARK --set-mark 0x01\n"
	" iptables -A FORWARD -p udp -m ipp2p --kazaa --bit -j DROP\n"
	" iptables -A FORWARD -p tcp -m ipp2p --edk --soul -j DROP\n\n"
	, IPP2P_VERSION);
}

static const struct option ipp2p_mt_opts[] = {
	{.name = "edk",   .has_arg = false, .val = '2'},
	{.name = "dc",    .has_arg = false, .val = '7'},
	{.name = "gnu",   .has_arg = false, .val = '9'},
	{.name = "kazaa", .has_arg = false, .val = 'a'},
	{.name = "bit",   .has_arg = false, .val = 'b'},
	{.name = "apple", .has_arg = false, .val = 'c'},
	{.name = "soul",  .has_arg = false, .val = 'd'},
	{.name = "winmx", .has_arg = false, .val = 'e'},
	{.name = "ares",  .has_arg = false, .val = 'f'},
	{.name = "mute",  .has_arg = false, .val = 'g'},
	{.name = "waste", .has_arg = false, .val = 'h'},
	{.name = "xdcc",  .has_arg = false, .val = 'i'},
	{.name = "debug", .has_arg = false, .val = 'j'},
	{NULL},
};

static int ipp2p_mt_parse(int c, char **argv, int invert, unsigned int *flags,
                          const void *entry, struct xt_entry_match **match)
{
	struct ipt_p2p_info *info = (struct ipt_p2p_info *)(*match)->data;

	switch (c) {
	case '2':		/*cmd: edk*/
		param_act(P_ONLY_ONCE, "--edk", *flags & IPP2P_EDK);
		param_act(P_NO_INVERT, "--edk", invert);
		if (*flags & IPP2P_DATA_EDK)
			exit_error(PARAMETER_PROBLEM,
				"ipp2p: use `--edk' OR `--edk-data' but not both of them!");
		*flags    |= IPP2P_EDK;
		info->cmd |= IPP2P_EDK;
		break;

	case '7':		/*cmd: dc*/
		param_act(P_ONLY_ONCE, "--dc", *flags & IPP2P_DC);
		param_act(P_NO_INVERT, "--dc", invert);
		if (*flags & IPP2P_DATA_DC)
			exit_error(PARAMETER_PROBLEM,
				"ipp2p: use `--dc' OR `--dc-data' but not both of them!");
		*flags    |= IPP2P_DC;
		info->cmd |= IPP2P_DC;
		break;

	case '9':		/*cmd: gnu*/
		param_act(P_ONLY_ONCE, "--gnu", *flags & IPP2P_GNU);
		param_act(P_NO_INVERT, "--gnu", invert);
		if (*flags & IPP2P_DATA_GNU)
			exit_error(PARAMETER_PROBLEM,
				"ipp2p: use `--gnu' OR `--gnu-data' but not both of them!");
		*flags    |= IPP2P_GNU;
		info->cmd |= IPP2P_GNU;
		break;

	case 'a':		/*cmd: kazaa*/
		param_act(P_ONLY_ONCE, "--kazaa", *flags & IPP2P_KAZAA);
		param_act(P_NO_INVERT, "--kazaa", invert);
		if (*flags & IPP2P_DATA_KAZAA)
			exit_error(PARAMETER_PROBLEM,
				"ipp2p: use `--kazaa' OR `--kazaa-data' but not both of them!");
		*flags    |= IPP2P_KAZAA;
		info->cmd |= IPP2P_KAZAA;
		break;

	case 'b':		/*cmd: bit*/
		param_act(P_ONLY_ONCE, "--kazaa", *flags & IPP2P_BIT);
		param_act(P_NO_INVERT, "--kazaa", invert);
		*flags    |= IPP2P_BIT;
		info->cmd |= IPP2P_BIT;
		break;

	case 'c':		/*cmd: apple*/
		param_act(P_ONLY_ONCE, "--apple", *flags & IPP2P_APPLE);
		param_act(P_NO_INVERT, "--apple", invert);
		*flags    |= IPP2P_APPLE;
		info->cmd |= IPP2P_APPLE;
		break;

	case 'd':		/*cmd: soul*/
		param_act(P_ONLY_ONCE, "--soul", *flags & IPP2P_SOUL);
		param_act(P_NO_INVERT, "--soul", invert);
		*flags    |= IPP2P_SOUL;
		info->cmd |= IPP2P_SOUL;
		break;

	case 'e':		/*cmd: winmx*/
		param_act(P_ONLY_ONCE, "--winmx", *flags & IPP2P_WINMX);
		param_act(P_NO_INVERT, "--winmx", invert);
		*flags    |= IPP2P_WINMX;
		info->cmd |= IPP2P_WINMX;
		break;

	case 'f':		/*cmd: ares*/
		param_act(P_ONLY_ONCE, "--ares", *flags & IPP2P_ARES);
		param_act(P_NO_INVERT, "--ares", invert);
		*flags    |= IPP2P_ARES;
		info->cmd |= IPP2P_ARES;
		break;

	case 'g':		/*cmd: mute*/
		param_act(P_ONLY_ONCE, "--mute", *flags & IPP2P_MUTE);
		param_act(P_NO_INVERT, "--mute", invert);
		*flags    |= IPP2P_MUTE;
		info->cmd |= IPP2P_MUTE;
		break;

	case 'h':		/*cmd: waste*/
		param_act(P_ONLY_ONCE, "--waste", *flags & IPP2P_WASTE);
		param_act(P_NO_INVERT, "--waste", invert);
		*flags    |= IPP2P_WASTE;
		info->cmd |= IPP2P_WASTE;
		break;

	case 'i':		/*cmd: xdcc*/
		param_act(P_ONLY_ONCE, "--xdcc", *flags & IPP2P_XDCC);
		param_act(P_NO_INVERT, "--xdcc", invert);
		*flags    |= IPP2P_XDCC;
		info->cmd |= IPP2P_XDCC;
		break;

	case 'j':		/*cmd: debug*/
		param_act(P_ONLY_ONCE, "--debug", info->debug);
		param_act(P_NO_INVERT, "--debug", invert);
		info->debug = 1;
		break;

	default:
//		exit_error(PARAMETER_PROBLEM,
//		"\nipp2p-parameter problem: for ipp2p usage type: iptables -m ipp2p --help\n");
		return 0;
	}
	return 1;
}

static void ipp2p_mt_check(unsigned int flags)
{
	if (!flags)
		exit_error(PARAMETER_PROBLEM,
			"\nipp2p-parameter problem: for ipp2p usage type: iptables -m ipp2p --help\n");
}

static const char *const ipp2p_cmds[] = {
	[IPP2N_EDK]        = "--edk",
	[IPP2N_DATA_KAZAA] = "--kazaa-data",
	[IPP2N_DATA_EDK]   = "--edk-data",
	[IPP2N_DATA_DC]    = "--dc-data",
	[IPP2N_DC]         = "--dc",
	[IPP2N_DATA_GNU]   = "--gnu-data",
	[IPP2N_GNU]        = "--gnu",
	[IPP2N_KAZAA]      = "--kazaa",
	[IPP2N_BIT]        = "--bit",
	[IPP2N_APPLE]      = "--apple",
	[IPP2N_SOUL]       = "--soul",
	[IPP2N_WINMX]      = "--winmx",
	[IPP2N_ARES]       = "--ares",
	[IPP2N_MUTE]       = "--mute",
	[IPP2N_WASTE]      = "--waste",
	[IPP2N_XDCC]       = "--xdcc",
};

static void
ipp2p_mt_print(const void *entry, const struct xt_entry_match *match,
               int numeric)
{
	const struct ipt_p2p_info *info = (const void *)match->data;
	unsigned int i;

	for (i = IPP2N_EDK; i <= IPP2N_XDCC; ++i)
		if (info->cmd & (1 << i))
			printf("%s ", ipp2p_cmds[i]);

	if (info->debug != 0)
		printf("--debug ");
}

static void ipp2p_mt_save(const void *entry, const struct xt_entry_match *match)
{
	ipp2p_mt_print(entry, match, true);
}

static struct xtables_match ipp2p_mt_reg = {
	.version       = XTABLES_VERSION,
	.name          = "ipp2p",
	.revision      = 0,
	.family        = AF_INET,
	.size          = XT_ALIGN(sizeof(struct ipt_p2p_info)),
	.userspacesize = XT_ALIGN(sizeof(struct ipt_p2p_info)),
	.help          = ipp2p_mt_help,
	.parse         = ipp2p_mt_parse,
	.final_check   = ipp2p_mt_check,
	.print         = ipp2p_mt_print,
	.save          = ipp2p_mt_save,
	.extra_opts    = ipp2p_mt_opts,
};

static void _init(void)
{
	xtables_register_match(&ipp2p_mt_reg);
}