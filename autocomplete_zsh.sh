_pax_autocomplete_zsh() {
	local entry label

	local -a dir_matches dir_display
	local -a file_matches file_display

	#command handler, paxpush does N+1 arguments
	case "${words[1]##*/}" in
		paxls|paxpull)
			(( CURRENT == 2 )) || {
				_files
				return
			}
			;;
	
		paxpush)
			(( CURRENT >= 3 && CURRENT == $#words )) || {
				_files
				return
			}
			;;
	
		*)
			return
			;;
	esac
	
	_pax_autocomplete_prepare "$PREFIX" || return 0
	
	while IFS= read -r entry; do
		[[ -n "$entry" ]] || continue
	
		label="${entry%/}"
		label="${label##*/}"
	
		if [[ "$entry" == */ ]]; then
			dir_matches+=("$entry")
			dir_display+=("${label}/")
		else
			file_matches+=("$entry")
			file_display+=("$label")
		fi
	done <<< "$_AUTOCOMP_MATCHES"
	
	(( $#dir_matches )) && compadd -f -S '' -d dir_display -- "${dir_matches[@]}"
	
	(( $#file_matches )) && compadd -f -d file_display -- "${file_matches[@]}"
}
