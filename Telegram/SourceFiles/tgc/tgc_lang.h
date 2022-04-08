/*
This file is part of TG Connect Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
LINK_PLACEHOLDER
*/
#pragma once

namespace TGC {
namespace Lang {

struct Var {
	Var() {};
	Var(const QString &k, const QString &v) {
		key = k;
		value = v;
	}

	QString key;
	QString value;
};

struct EntVar {
	EntVar() {};
	EntVar(const QString &k, TextWithEntities v) {
		key = k;
		value = v;
	}

	QString key;
	TextWithEntities value;
};

void Load(const QString &baseLangCode, const QString &langCode);

QString Translate(
	const QString &key,
	Var var1 = Var(),
	Var var2 = Var(),
	Var var3 = Var(),
	Var var4 = Var());
QString Translate(
	const QString &key,
	float64 value,
	Var var1 = Var(),
	Var var2 = Var(),
	Var var3 = Var(),
	Var var4 = Var());

TextWithEntities TranslateWithEntities(
	const QString &key,
	EntVar var1 = EntVar(),
	EntVar var2 = EntVar(),
	EntVar var3 = EntVar(),
	EntVar var4 = EntVar());
TextWithEntities TranslateWithEntities(
	const QString &key,
	float64 value,
	EntVar var1 = EntVar(),
	EntVar var2 = EntVar(),
	EntVar var3 = EntVar(),
	EntVar var4 = EntVar());

rpl::producer<> Events();

} // namespace Lang
} // namespace TGC

// Shorthands

inline QString ctr(
	const QString &key,
	::TGC::Lang::Var var1 = ::TGC::Lang::Var(),
	::TGC::Lang::Var var2 = ::TGC::Lang::Var(),
	::TGC::Lang::Var var3 = ::TGC::Lang::Var(),
	::TGC::Lang::Var var4 = ::TGC::Lang::Var()) {
	return ::TGC::Lang::Translate(key, var1, var2, var3, var4);
}

inline QString ctr(
	const QString &key,
	float64 value,
	::TGC::Lang::Var var1 = ::TGC::Lang::Var(),
	::TGC::Lang::Var var2 = ::TGC::Lang::Var(),
	::TGC::Lang::Var var3 = ::TGC::Lang::Var(),
	::TGC::Lang::Var var4 = ::TGC::Lang::Var()) {
	return ::TGC::Lang::Translate(key, value, var1, var2, var3, var4);
}

inline TextWithEntities ctre(
	const QString &key,
	::TGC::Lang::EntVar var1 = ::TGC::Lang::EntVar(),
	::TGC::Lang::EntVar var2 = ::TGC::Lang::EntVar(),
	::TGC::Lang::EntVar var3 = ::TGC::Lang::EntVar(),
	::TGC::Lang::EntVar var4 = ::TGC::Lang::EntVar()) {
	return ::TGC::Lang::TranslateWithEntities(key, var1, var2, var3, var4);
}

inline TextWithEntities ctre(
	const QString &key,
	float64 value,
	::TGC::Lang::EntVar var1 = ::TGC::Lang::EntVar(),
	::TGC::Lang::EntVar var2 = ::TGC::Lang::EntVar(),
	::TGC::Lang::EntVar var3 = ::TGC::Lang::EntVar(),
	::TGC::Lang::EntVar var4 = ::TGC::Lang::EntVar()) {
	return ::TGC::Lang::TranslateWithEntities(key, value, var1, var2, var3, var4);
}

inline rpl::producer<QString> rctr(
	const QString &key,
	::TGC::Lang::Var var1 = ::TGC::Lang::Var(),
	::TGC::Lang::Var var2 = ::TGC::Lang::Var(),
	::TGC::Lang::Var var3 = ::TGC::Lang::Var(),
	::TGC::Lang::Var var4 = ::TGC::Lang::Var()) {
	return rpl::single(
			::TGC::Lang::Translate(key, var1, var2, var3, var4)
		) | rpl::then(
			::TGC::Lang::Events() | rpl::map(
				[=]{ return ::TGC::Lang::Translate(key, var1, var2, var3, var4); })
		);
}

inline rpl::producer<QString> rctr(
	const QString &key,
	float64 value,
	::TGC::Lang::Var var1 = ::TGC::Lang::Var(),
	::TGC::Lang::Var var2 = ::TGC::Lang::Var(),
	::TGC::Lang::Var var3 = ::TGC::Lang::Var(),
	::TGC::Lang::Var var4 = ::TGC::Lang::Var()) {
	return rpl::single(
			::TGC::Lang::Translate(key, value, var1, var2, var3, var4)
		) | rpl::then(
			::TGC::Lang::Events() | rpl::map(
				[=]{ return ::TGC::Lang::Translate(key, value, var1, var2, var3, var4); })
		);
}

inline rpl::producer<TextWithEntities> rctre(
	const QString &key,
	::TGC::Lang::EntVar var1 = ::TGC::Lang::EntVar(),
	::TGC::Lang::EntVar var2 = ::TGC::Lang::EntVar(),
	::TGC::Lang::EntVar var3 = ::TGC::Lang::EntVar(),
	::TGC::Lang::EntVar var4 = ::TGC::Lang::EntVar()) {
	return rpl::single(
			::TGC::Lang::TranslateWithEntities(key, var1, var2, var3, var4)
		) | rpl::then(
			::TGC::Lang::Events() | rpl::map(
				[=]{ return ::TGC::Lang::TranslateWithEntities(key, var1, var2, var3, var4); })
		);
}

inline rpl::producer<TextWithEntities> rctre(
	const QString &key,
	float64 value,
	::TGC::Lang::EntVar var1 = ::TGC::Lang::EntVar(),
	::TGC::Lang::EntVar var2 = ::TGC::Lang::EntVar(),
	::TGC::Lang::EntVar var3 = ::TGC::Lang::EntVar(),
	::TGC::Lang::EntVar var4 = ::TGC::Lang::EntVar()) {
	return rpl::single(
			::TGC::Lang::TranslateWithEntities(key, value, var1, var2, var3, var4)
		) | rpl::then(
			::TGC::Lang::Events() | rpl::map(
				[=]{ return ::TGC::Lang::TranslateWithEntities(key, value, var1, var2, var3, var4); })
		);
}
