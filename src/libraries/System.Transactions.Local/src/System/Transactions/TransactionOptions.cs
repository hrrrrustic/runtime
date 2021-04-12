// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

using System.Diagnostics.CodeAnalysis;

namespace System.Transactions
{
    public struct TransactionOptions
    {
        private TimeSpan _timeout;
        private IsolationLevel _isolationLevel;

        public TimeSpan Timeout
        {
            readonly get { return _timeout; }
            set { _timeout = value; }
        }

        public IsolationLevel IsolationLevel
        {
            readonly get { return _isolationLevel; }
            set { _isolationLevel = value; }
        }

        public override readonly int GetHashCode() => base.GetHashCode();  // Don't have anything better to do.

        public override readonly bool Equals([NotNullWhen(true)] object? obj) => obj is TransactionOptions transactionOptions && Equals(transactionOptions);

        private readonly bool Equals(TransactionOptions other) =>
            _timeout == other._timeout &&
            _isolationLevel == other._isolationLevel;

        public static bool operator ==(TransactionOptions x, TransactionOptions y) => x.Equals(y);

        public static bool operator !=(TransactionOptions x, TransactionOptions y) => !x.Equals(y);
    }
}
