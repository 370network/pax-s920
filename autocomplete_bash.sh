_pax_autocomplete_bash() {
	local last_word=$((${#COMP_WORDS[@]} - 1))
	local entry has_dirs=0

	COMPREPLY=()

	case "${COMP_WORDS[0]##*/}" in
		paxls|paxpull)
			(( COMP_CWORD == 1 )) || return 0
			;;

		paxpush)
			(( COMP_CWORD >= 2 && COMP_CWORD == last_word )) || return 0
			;;

		*)
			return 0
			;;
	esac

	_pax_autocomplete_prepare "${COMP_WORDS[COMP_CWORD]}" || return 0

	while IFS= read -r entry; do
		[[ -n "$entry" ]] || continue

		COMPREPLY+=("$entry")

		[[ "$entry" == */ ]] && has_dirs=1
	done <<< "$_AUTOCOMP_MATCHES"

	compopt -o filenames 2>/dev/null || :

	(( has_dirs )) && compopt -o nospace 2>/dev/null || :
}